#pragma once

#include <cstddef>
#include <cstdint>
#include <chrono>

namespace Utils::Time {
    inline uint64_t get_time_ms() {
        using namespace std::chrono;
        
        auto now = steady_clock::now();
        
        return duration_cast<milliseconds>(now.time_since_epoch()).count();
    }

    inline uint64_t get_time_us() {
        using namespace std::chrono;

        auto now = steady_clock::now();
        
        return duration_cast<microseconds>(now.time_since_epoch()).count();
    }
}
