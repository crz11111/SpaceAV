#include <iostream>
#include <DataFrame/DataFrame.h>
#include <bitset>

#include "Morton.hpp"
#include "Hilbert.hpp"

using namespace hmdf;

using iDataFrame = StdDataFrame<int>;

std::pair<uint64_t, uint64_t>
ComputeHilbertThres(double shift_unit, double x1, double y1, double z1, double x2, double y2, double z2) {
    std::vector<uint32_t> up_bound = {(uint32_t) ((x1 + shift_unit) * 10), (uint32_t) ((y1 + shift_unit) * 10),
                                      (uint32_t) ((z1 + shift_unit) * 10)};
    std::vector<uint32_t> down_bound = {(uint32_t) ((x2 + shift_unit) * 10), (uint32_t) ((y2 + shift_unit) * 10),
                                        (uint32_t) ((z2 + shift_unit) * 10)};
    return std::pair<uint64_t, uint64_t>(HilbertEncode(up_bound), HilbertEncode(down_bound));
}

void DataFrameToCSV(iDataFrame df, const char *output_path) {
    std::ofstream loaded_result;
    loaded_result.open(output_path);
    df.write<std::ostream, long, double, int>(loaded_result, io_format::csv2);
    loaded_result.close();
}

std::vector<std::pair<long, long>> EventsQuery(const std::vector<long> &hilbert_query, uint64_t duration) {
    std::pair<long, long> event(-1, -1);
    std::vector<std::pair<long, long>> query_events;
    for (long timestamp_index: hilbert_query) {
        if (event.first == -1) {
            event.first = timestamp_index;
            continue;
        }
        if (timestamp_index - event.first <= duration) {
            event.second = timestamp_index;
            if (timestamp_index - event.first == duration) {
                query_events.push_back(event);
                event = std::pair<long, long>(-1, -1);
            }
        } else {
            if (event.second != -1) {
                query_events.push_back(event);
            }
            event.first = timestamp_index;
        }
    }
    return query_events;
}

int main(int argc, char **argv) {
    if (argc != 3 && argc != 11) {
        std::cout
                << "Wrong arguments, please specify your data source as: \n "
                   "sfc <ORIGIN_DATA_PATH> <OUTPUT_PATH> \n"
                   "-<option> <accel_lon1> <accel_trans1> <accel_down1> <accel_lon2> <accel_trans2> <accel_down2> <duration> \n"
                << std::endl;
        return 1;
    }

    // Read original csv data
    const char *data_path = argv[1];
    std::string output_path(argv[2]);
    const uint64_t duration = std::stol(argv[10]);

    iDataFrame df;

    std::cout << "Data input source: " << data_path << std::endl;
    std::cout << "Data output source: " << output_path << std::endl;

    df.read(data_path, io_format::csv2);
    const auto &accel_lon_col_ref = df.get_column<double>("accel_lon");
    const auto &accel_trans_col_ref = df.get_column<double>("accel_trans");
    const auto &accel_down_col_ref = df.get_column<double>("accel_down");
    std::vector<int> hilbert_coll, morton_coll;

    //Shifting unit for abstraction in unsigned range
    std::vector<double> min_accel = {
            *min_element(accel_lon_col_ref.begin(), accel_lon_col_ref.end()),
            *min_element(accel_trans_col_ref.begin(), accel_trans_col_ref.end()),
            *min_element(accel_down_col_ref.begin(), accel_down_col_ref.end())
    };
    double shift_unit = *min_element(min_accel.begin(), min_accel.end());
    if (shift_unit < 0) {
        shift_unit = shift_unit * -1;
    }

    // Calculate Hilbert for inputted query range
    std::pair<uint64_t, uint64_t> query_range = ComputeHilbertThres(
            shift_unit,
            std::stod(argv[4]),
            std::stod(argv[5]),
            std::stod(argv[6]),
            std::stod(argv[7]),
            std::stod(argv[8]),
            std::stod(argv[9])
    );
    if (query_range.first > query_range.second) {
        query_range.swap(query_range);
    }

    // Computing Morton and Hilbert indexes
    for (size_t i = 0; i < accel_lon_col_ref.size(); ++i) {
        uint32_t lon, trans, down;
        uint64_t morton, hilbert;

        // Calculate Morton
        lon = (accel_lon_col_ref[i] + shift_unit) * 100;
        trans = (accel_trans_col_ref[i] + shift_unit) * 100;

        morton = mortonEncode(std::pair<uint32_t, uint32_t>(lon, trans));

        // Calculate Hilbert (keep 1 Decimal)
        lon = (accel_lon_col_ref[i] + shift_unit) * 10;
        trans = (accel_trans_col_ref[i] + shift_unit) * 10;
        down = (accel_down_col_ref[i] + shift_unit) * 10;

        std::vector<uint32_t> X = {lon, trans, down};
        hilbert = HilbertEncode(X);

        std::cout << "- Shifted data value pair: " << accel_lon_col_ref[i] + 5 << " " << accel_trans_col_ref[i] + 5
                  << " " << accel_down_col_ref[i] + 5
                  << " ---> Hilbert integer = " << std::bitset<21>(hilbert) << " = " << hilbert
                  << " ---> Morton Code = " << std::bitset<64>(morton) << " = " << morton
                  << std::endl;

        morton_coll.push_back(morton);
        hilbert_coll.push_back(hilbert);
    }

    //Output converted representation/index
    df.load_column<int>("morton_index", move(morton_coll));
    df.load_column<int>("hilbert_index", move(hilbert_coll));
    DataFrameToCSV(df, output_path.append("/loaded_indexes.csv").c_str());
    std::cout << "Shifted unit: " << shift_unit << std::endl;
    std::cout << "New indexes saved to " << output_path << std::endl;
    output_path.assign(argv[2]);

    // Query Hilbert indexes
    // TO-DO Multi-Thread sorting
    df.sort<int, long, double, int>("hilbert_index", sort_spec::ascen);
    auto functor = [&query_range](const int &, const int &val1) -> bool {
        return (val1 >= query_range.first && val1 <= query_range.second);
    };
    auto query_result = df.get_data_by_sel<int, decltype(functor), long, double, int>("hilbert_index", functor);
    query_result.sort<long, int, long, double, int>("timestamp", sort_spec::ascen);

    // Output converted representation/index
    DataFrameToCSV(query_result, output_path.append("/query_result.csv").c_str());
    std::cout << "Query range: " << query_range.first << " <-> " << query_range.second << std::endl;
    std::cout << "Query result saved to " << output_path << std::endl;
    output_path.assign(argv[2]);

    // Computing event query results
    const std::vector<long> &query_result_timestamp_col_ref = query_result.get_column<long>("timestamp");
    const std::vector<std::pair<long, long>> query_events = EventsQuery(query_result_timestamp_col_ref, duration);

    // Output query result for events
    std::cout << "Query found events: " << std::endl;
    int l = 0;
    for (std::pair<long, long> suggested_event: query_events) {
        l++;
        std::cout << "Event " <<  l << " (" << suggested_event.first << ", " << suggested_event.second << ") " << std::endl;
    }
    if (l == 0) {
        std::cout << "-----No events found-----" << std::endl;
    }

    return 0;
}