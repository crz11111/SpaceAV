/*
 * Copyright (C) 2015, 2019 Dawid Szymański, Todd Lehman, Johan
 *
 * Source: https://stackoverflow.com/questions/30539347/2d-morton-code-encode-decode-64bits
 *
 * CC BY-SA 4.0
 */

#ifndef MORTON_HPP
#define MORTON_HPP

#include <utility>
#include <cmath>
#include <cstdint>

inline uint64_t mortonEncode(const std::pair<std::uint32_t,std::uint32_t> &xy) {
    uint64_t x = xy.first;
    uint64_t y = xy.second;
    x = (x | (x << 16)) & 0x0000FFFF0000FFFF;
    x = (x | (x << 8)) & 0x00FF00FF00FF00FF;
    x = (x | (x << 4)) & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x << 2)) & 0x3333333333333333;
    x = (x | (x << 1)) & 0x5555555555555555;

    y = (y | (y << 16)) & 0x0000FFFF0000FFFF;
    y = (y | (y << 8)) & 0x00FF00FF00FF00FF;
    y = (y | (y << 4)) & 0x0F0F0F0F0F0F0F0F;
    y = (y | (y << 2)) & 0x3333333333333333;
    y = (y | (y << 1)) & 0x5555555555555555;

    const uint64_t result = x | (y << 1);
    return result;
}

inline uint32_t mortonExtractEvenBits(uint64_t x) {
    x = x & 0x5555555555555555;
    x = (x | (x >> 1))  & 0x3333333333333333;
    x = (x | (x >> 2))  & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4))  & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8))  & 0x0000FFFF0000FFFF;
    x = (x | (x >> 16)) & 0x00000000FFFFFFFF;
    return static_cast<uint32_t>(x);
}

inline std::pair<std::uint32_t,std::uint32_t> mortonDecode(uint64_t code) {
    const uint32_t x = mortonExtractEvenBits(code);
    const uint32_t y = mortonExtractEvenBits(code >> 1);
    return std::make_pair(x, y);
}

inline uint64_t convertLatLonToMorton(const std::pair<float,float> &coordinate) {
    const uint32_t _lat = std::lroundf((coordinate.first + 90.0f) * 100000);
    const uint32_t _lon = std::lround((coordinate.second + 180.0f) * 100000);
    const auto _tmp = std::make_pair(_lat, _lon);
    return mortonEncode(_tmp);
}

inline std::pair<float,float> convertMortonToLatLon(uint64_t code) {
    auto tmp = mortonDecode(code);
    const float _lat = tmp.first/100000.0f - 90.0f;
    const float _lon = tmp.second/100000.0f - 180.0f;
    return std::make_pair(_lat, _lon);
}

#endif //SFC_MORTONEXPERIMENT_HPP
