#include "consumer.hpp"

#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <broker.hpp>
#include <thread>
#include <common/bytes.hpp>


Consumer::Consumer(Broker* broker) {
    if (broker != nullptr) {
        this->broker = broker;
    }
}


void Consumer::loop() {
    for (int i = 0; i < 10; ++i) {
        std::vector<uint8_t> buffer(256);
        size_t received_size = 0;

        if (this->broker->recv(buffer.data(), buffer.size(), received_size)) {
            std::cout << "Message received successfully! (" << received_size << " bytes)\n";
            std::cout << "Data: ";
            
            Utils::Bytes::print_bytes(buffer, received_size);
        } else {
            std::cerr << "Failed to receive message.\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "[Consumer] Finished work.\n";
}
