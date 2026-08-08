#pragma once
 
#include <cstddef>
#include <cstdint>

#define MAX_SIZE 255


#pragma pack(push, 1)
struct Packet {
    uint16_t size;
    uint8_t payload[MAX_SIZE];
};
#pragma pack(pop)


class Transport {
    public:
        virtual ~Transport() = default;

        virtual bool send(const Packet* packet) = 0;
        virtual bool recv(Packet* packet) = 0;
};
