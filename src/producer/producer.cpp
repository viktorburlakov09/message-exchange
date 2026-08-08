#include "producer.hpp"

#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <thread>
#include <bytes.hpp>
#include <control.hpp>


Producer::Producer(Broker* broker, size_t packet_size) {
    if (broker != nullptr) {
        this->broker = broker;
    }
    this->packet_size = packet_size;
}


void Producer::loop()
{
    ControlHandler control;

    int i = 0;
    while (control.is_running() && i < 100) {
        if (control.paused()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        auto payload = generate_random_bytes(packet_size);
        // auto payload = this->generate_sequence_bytes(this->packet_size);
        bool success = this->broker->send(payload.data(), payload.size());
        
        if (success) {
            std::cout << "[Producer] Sent packet #" << (i + 1) << " of size " << this->packet_size << " bytes.\n";
            Utils::Bytes::print_bytes(payload, this->packet_size);
        } else {
            std::cerr << "[Producer] Failed to send packet #" << (i + 1) << "\n";
        }

        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "[Producer] Finished work.\n";
}



std::vector<uint8_t> Producer::generate_random_bytes(size_t length) {
    std::vector<uint8_t> buffer(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    for (size_t i = 0; i < length; ++i) {
        buffer[i] = static_cast<uint8_t>(dis(gen));
    }

    return buffer;
}


std::vector<uint8_t> Producer::generate_sequence_bytes(size_t length) {
    std::vector<uint8_t> buffer(length);

    for (size_t i = 0; i < length; ++i) {
        buffer[i] = static_cast<uint8_t>(i & 0xFF);
    }

    return buffer;
}
