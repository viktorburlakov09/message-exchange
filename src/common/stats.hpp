#pragma once

#include <iostream>
#include <chrono>
#include <string>


class TrafficStats {
    public:
        explicit TrafficStats(std::string app_prefix = "[Stats]") : prefix(std::move(app_prefix)) {}

        void add_packet(size_t packet_bytes) {
            this->total_packets++;
            this->packets_this_second++;
            this->bytes_this_second += packet_bytes;
        }

        void update() {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - this->last_time).count();

            if (elapsed >= 1) {
                double pkts_per_sec = static_cast<double>(this->packets_this_second) / elapsed;
                double bytes_per_sec = static_cast<double>(this->bytes_this_second) / elapsed;

                std::cout << prefix << " Total: " << this->total_packets
                        << " | Pkts/sec: " << pkts_per_sec 
                        << " | Bytes/sec: " << static_cast<uint64_t>(bytes_per_sec) << " B/s\n";

                this->packets_this_second = 0;
                this->bytes_this_second = 0;
                this->last_time = current_time;
            }
        }

        void reset_timer() {
            this->last_time = std::chrono::steady_clock::now();
        }

    private:
        size_t total_packets = 0;
        size_t packets_this_second = 0;
        size_t bytes_this_second = 0;

        std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
        std::string prefix;
};
