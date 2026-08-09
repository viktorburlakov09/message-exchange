#pragma once

#include <cstddef>
#include <cstdint>
#include <chrono>

namespace Utils::CRC {
    
    inline uint8_t calculate_crc(const uint8_t* data, size_t size) {
        uint8_t crc = 0;
        size_t step = (size > 1024) ? (size / 64) : 1;

        for (size_t i = 0; i < size; i += step) {
            crc ^= data[i];
        }
        
        return crc;
    }

    inline bool verify_crc(const uint8_t* data, size_t size, uint8_t expected_crc) {
        uint8_t actual_crc = calculate_crc(data, size);
        return actual_crc == expected_crc;
    }

}
