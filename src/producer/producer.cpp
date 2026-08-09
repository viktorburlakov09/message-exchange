#include "producer.hpp"

#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <thread>
#include <bytes.hpp>
#include <signal_handler.hpp>
#include <keyboard_handler.hpp>
#include <random_generator.hpp>


Producer::Producer(Broker* broker, size_t packet_size) {
    if (broker != nullptr) {
        this->broker = broker;
    }
    this->packet_size = packet_size;
}


void Producer::loop() {
    int i = 0;
    KeyboardHandler key_handler;
    RandomGenerator gen(this->packet_size);

    while (!SignalHandler::is_shutdown() && key_handler.is_running()) {
       if (key_handler.is_paused()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        const auto& payload = gen.next();
        bool success = this->broker->send(payload.data(), payload.size());
        
        if (success) {
            // std::cout << "[Producer] Sent packet #" << (i + 1) << " of size " << this->packet_size << " bytes.\n";
            // Utils::Bytes::print_bytes(payload, this->packet_size);
        }

        i++;
    }

    std::cout << "[Producer] Finished work.\n";
}
