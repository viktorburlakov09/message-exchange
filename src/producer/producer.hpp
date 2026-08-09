#pragma once

#include <string>
#include <broker.hpp>
#include <random>


class Producer {
    public:
        void loop();

        Producer(Broker* broker, size_t packet_size);

    private:
        size_t packet_size;
        Broker* broker;

        std::vector<uint8_t> generate_random_bytes(size_t length);
        std::vector<uint8_t> generate_sequence_bytes(size_t length);
};
