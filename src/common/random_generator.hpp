#pragma once

#include <vector>
#include <cstdint>
#include <random>
#include <cstring>

#pragma once

#include <vector>
#include <cstdint>
#include <random>
#include <cstring>

class RandomGenerator {
    public:
        explicit RandomGenerator(size_t packet_size) : buffer(packet_size), state(123456789) {
            std::random_device rd;
            uint32_t seed = rd();
            
            if (seed != 0) {
                this->state = seed;
            }
        }

        const std::vector<uint8_t>& next() {
            size_t length = this->buffer.size();
            uint64_t* ptr = reinterpret_cast<uint64_t*>(this->buffer.data());
            
            uint32_t x = this->state;
            size_t i = 0;
            
            for (; i + 7 < length; i += 8) {
                x ^= x << 13; x ^= x >> 17; x ^= x << 5;
                uint64_t low = x;
                
                x ^= x << 13; x ^= x >> 17; x ^= x << 5;
                uint64_t high = x;

                *ptr++ = (high << 32) | low;
            }
            this->state = x;

            if (i < length) {
                x ^= x << 13; x ^= x >> 17; x ^= x << 5;
                std::memcpy(this->buffer.data() + i, &x, length - i);
                this->state = x;
            }

            return this->buffer;
        }

    private:
        uint32_t state;
        std::vector<uint8_t> buffer;
};


class SequenceGenerator {
    public:
        SequenceGenerator() = default;

        std::vector<uint8_t> generate(size_t length) {
            std::vector<uint8_t> buffer(length);
            
            for (size_t i = 0; i < length; ++i) {
                buffer[i] = static_cast<uint8_t>(i & 0xFF);
            }
            
            return buffer;
        }
};
