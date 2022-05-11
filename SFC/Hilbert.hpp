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

void HilbertDecode(int hilbert_integer) // Hilbert integer
{
    // TO-DO Decoding Hilbert integers
}

#endif //SFC_HILBERT_HPP