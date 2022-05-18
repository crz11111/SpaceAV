//
// Created by Coulson Chen on 2022-05-18.
//

#include "catch.hpp"
#include "Hilbert.hpp"

TEST_CASE("Test encode/decode") {
    std::vector<std::uint32_t> xyz1 = {55, 63, 119};
    std::uint64_t mc1 = HilbertEncode(xyz1);
    std::vector<std::uint32_t> d1 = HilbertDecode(mc1);
    //std::cout << xy1.first << " " << xy1.second << " " << mc1 << ", " << d1.first << "," << d1.second << std::endl;
    REQUIRE(xyz1[0] == d1[0]);
    REQUIRE(xyz1[1] == d1[1]);
    REQUIRE(xyz1[2] == d1[2]);
    REQUIRE(mc1 == 521727);

    std::vector<std::uint32_t> xyz2 = {114, 26, 79};
    std::uint64_t mc2 = HilbertEncode(xyz2);
    std::vector<std::uint32_t> d2 = HilbertDecode(mc2);
    //std::cout << xy2.first << " " << xy2.second << " " << mc2 << ", " << d2.first << ", " << d2.second << std::endl;
    REQUIRE(xyz2[0] == d2[0]);
    REQUIRE(xyz2[1] == d2[1]);
    REQUIRE(xyz2[2] == d2[2]);
    REQUIRE(mc2 == 1468025);

    std::vector<std::uint32_t> xyz3 = {99, 103, 120};
    std::uint64_t mc3 = HilbertEncode(xyz3);
    std::vector<std::uint32_t> d3 = HilbertDecode(mc3);
    //std::cout << xy2.first << " " << xy2.second << " " << mc2 << ", " << d2.first << ", " << d2.second << std::endl;
    REQUIRE(xyz3[0] == d3[0]);
    REQUIRE(xyz3[1] == d3[1]);
    REQUIRE(xyz3[2] == d3[2]);
    REQUIRE(mc3 == 2069174);
}

TEST_CASE("Test range query") {

}



