#pragma once
 
#include <cstddef>
#include <cstdint>


#pragma pack(push, 1)
struct MessageHeader {
    uint64_t timestamp;
    uint8_t crc;
    uint16_t size;
};
#pragma pack(pop)
