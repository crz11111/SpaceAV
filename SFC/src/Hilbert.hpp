#ifndef HILBERT_HPP
#define HILBERT_HPP

#include <cstdint>
#include <bitset>
#include <iostream>
#include <vector>

#define HILBERT_SIZE 24 //Hilbert integer size in bits

uint64_t HilbertEncode(std::vector<uint32_t> X) // lon, trans, down
{
    std::bitset<HILBERT_SIZE> hilbert;

    for (int j = HILBERT_SIZE - 1; j >= 0; j--) {
        hilbert[j] = X[2 - j % 3] >> j / 3 & 1;
    }

    return hilbert.to_ulong();
}

std::vector<uint32_t> HilbertDecode(uint64_t hilbert_integer) // Hilbert integer
{
    std::bitset<HILBERT_SIZE> hilbert(hilbert_integer);
    std::bitset<HILBERT_SIZE / 3> x, y, z;
    std::vector<std::bitset<HILBERT_SIZE / 3>> X = {x, y, z};

    for (int j = 0; j < HILBERT_SIZE; ++j) {
        X[j % 3][(HILBERT_SIZE - j - 1) / 3] = hilbert[HILBERT_SIZE - j - 1];
    }

    return std::vector<uint32_t>{static_cast<unsigned int>(X[0].to_ulong()),
                                 static_cast<unsigned int>(X[1].to_ulong()),
                                 static_cast<unsigned int>(X[2].to_ulong())};
}

#endif //SFC_HILBERT_HPP