#include <iostream>
#include <fstream>
#include <DataFrame/DataFrame.h>

#include "MortonExperiment.hpp"

using namespace std;
using namespace hmdf;

using iDataFrame = StdDataFrame<int>;

int main() {
    iDataFrame df;
    df.read("/Users/coulsonchen/CLionProjects/SFC/data/preview_data.csv", io_format::csv2);
    const auto &accel_lon_col_ref = df.get_column<int>("accel_lon_reduced");
    const auto &accel_trans_col_ref = df.get_column<int>("accel_trans_reduced");
    std::vector<int> morton_coll;
    std::cout << std::endl;
    for (std::size_t i = 0; i < accel_lon_col_ref.size(); ++i){
        unsigned int x = *&accel_lon_col_ref[i];
        unsigned int y = *&accel_trans_col_ref[i];
        morton_coll.push_back(int(mortonEncode(pair<std::uint32_t,std::uint32_t>(x,y))));
        if(x>0 || y>0) {
            printf("%d, ", mortonEncode(pair<std::uint32_t, std::uint32_t>(x, y)));
        }
    }

    df.load_column<int>("morton_code", std::move(morton_coll));
    std::ofstream loaded_morton;
    loaded_morton.open("/Users/coulsonchen/CLionProjects/SFC/data/loaded_morton.csv");
    df.write<std::ostream, long, double, int>(loaded_morton, io_format::csv2);
    loaded_morton.close();
    return 0;
}