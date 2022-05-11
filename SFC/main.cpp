#include <iostream>
#include <DataFrame/DataFrame.h>
#include <bitset>

#include "MortonExperiment.hpp"
#include "Hilbert.hpp"

using namespace hmdf;

using iDataFrame = StdDataFrame<int>;

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout
                << "Wrong arguments, please specify your data source as: \n sfc <ORIGIN_DATA_PATH> <OUTPUT_PATH>"
                << std::endl;
        return 1;
    }

    // Read original csv data
    const char *data_path = argv[1];
    const char *output_path = argv[2];
    iDataFrame df;
    std::cout << "Data input source: " << data_path << std::endl;
    std::cout << "Data output source: " << output_path << std::endl;
    df.read(data_path, io_format::csv2);

    const auto &accel_lon_col_ref = df.get_column<double>("accel_lon");
    const auto &accel_trans_col_ref = df.get_column<double>("accel_trans");
    const auto &accel_down_col_ref = df.get_column<double>("accel_down");
    std::vector<int> hilbert_coll, morton_coll;
    std::vector<double> min_accel = {
            *min_element(accel_lon_col_ref.begin(), accel_lon_col_ref.end()),
            *min_element(accel_trans_col_ref.begin(), accel_trans_col_ref.end()),
            *min_element(accel_down_col_ref.begin(), accel_down_col_ref.end())
    };

    //Shifting unit for abstraction in unsigned range
    double shift_unit = *min_element(min_accel.begin(), min_accel.end());

    if (shift_unit < 0) {
        shift_unit = shift_unit * -1;
    }

    std::cout << shift_unit << std::endl;

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
    std::ofstream loaded_result;
    loaded_result.open(output_path);
    df.write<std::ostream, long, double, int>(loaded_result, io_format::csv2);
    loaded_result.close();

    std::cout << "Shifted unit:  " << shift_unit << std::endl;
    std::cout << "New indexes saved to " << output_path << std::endl;

    return 0;
}