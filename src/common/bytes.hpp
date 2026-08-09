#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdint>


namespace Utils::Bytes {

    inline void print_bytes(const std::vector<uint8_t>& data, size_t size) {
        std::cout << "[Data Hex]: ";

        size_t limit = std::min(size, data.size());

        for (size_t i = 0; i < limit; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                    << static_cast<int>(data[i]) << " ";
        }

        std::cout << std::dec << "\n";
    }

}