#include <iostream>
#include <DataFrame/DataFrame.h>

#include "MortonExperiment.hpp"
#include "Hilbert.hpp"

using namespace std;
using namespace hmdf;

using iDataFrame = StdDataFrame<int>;

#define HILBERT_SIZE 24 //Hilbert integer size in bits

int main() {
    // Read original csv data
    iDataFrame df;
    df.read("/Users/coulsonchen/Downloads/SpaceAV/SFC/data/preview_data.csv", io_format::csv2);

    const auto &accel_lon_col_ref = df.get_column<double>("accel_lon");
    const auto &accel_trans_col_ref = df.get_column<double>("accel_trans");
    const auto &accel_down_col_ref = df.get_column<double>("accel_down");
    vector<int> data_representation_coll;
    vector<double> min_accel = {
            *min_element(accel_lon_col_ref.begin(), accel_lon_col_ref.end()),
            *min_element(accel_trans_col_ref.begin(), accel_trans_col_ref.end()),
            *min_element(accel_down_col_ref.begin(), accel_down_col_ref.end())
    };

    //Shifting unit for abstraction in unsigned range
    const auto shift_unit = *min_element(min_accel.begin(), min_accel.end());

    for (size_t i = 0; i < accel_lon_col_ref.size(); ++i) {
        uint32_t lon, trans, down;

        /*
        // Calculate Morton
        lon = (accel_lon_col_ref[i]+5)*100;
        trans = (accel_trans_col_ref[i]+5)*100;
        down = (accel_down_col_ref[i]+5)*100;

        data_representation_coll.push_back(int(mortonEncode(pair<uint32_t,uint32_t>(x,y))));
        printf("%d, ", int(mortonEncode(pair<uint32_t, uint32_t>(x, y))));
        */

        // Calculate Hilbert (keep 1 Decimal)
        lon = (accel_lon_col_ref[i] + abs(shift_unit)) * 10;
        trans = (accel_trans_col_ref[i] + abs(shift_unit)) * 10;
        down = (accel_down_col_ref[i] + abs(shift_unit)) * 10;

        vector<uint32_t> X = {lon, trans, down};

        bitset<HILBERT_SIZE> hilbert;

        for (int j = HILBERT_SIZE - 1; j >= 0; j--) {
            hilbert[j] = X[2 - j % 3] >> j / 3 & 1;
        }

        cout << " " << lon << " " << trans << " " << down
             << " Hilbert integer = " << hilbert.to_string() << " = " << int(hilbert.to_ulong()) << " check"
             << endl;
        data_representation_coll.push_back(int(hilbert.to_ulong()));
    }

    //Output converted Morton representation
    df.load_column<int>("reduced_index", move(data_representation_coll));
    ofstream loaded_morton;
    loaded_morton.open("/Users/coulsonchen/Downloads/SpaceAV/SFC/data/loaded_Hilbert_representation.csv");
    df.write<ostream, long, double, int>(loaded_morton, io_format::csv2);
    loaded_morton.close();

    return 0;
}