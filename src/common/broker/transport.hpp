#pragma once
 
#include <cstddef>
#include <cstdint>


class Transport {
    public:
        virtual ~Transport() = default;

        virtual bool send(const uint8_t* data, size_t size) = 0;
        virtual bool recv(uint8_t* buffer, size_t max_size, size_t& out_size) = 0;
};
