#pragma once

#include <cstddef>
#include <cstdint>
#include <chrono>

namespace Utils::CRC {
    
    uint8_t calculate_crc(const uint8_t* data, size_t size) {
        uint8_t crc = 0;
        
        for (size_t i = 0; i < size; ++i) {
            crc ^= data[i];
        }
        
        return crc;
    }

}
