#include "consumer.hpp"

#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <broker.hpp>
#include <thread>
#include <common/bytes.hpp>
#include <keyboard_handler.hpp>
#include <signal_handler.hpp>


Consumer::Consumer(Broker* broker) {
    if (broker != nullptr) {
        this->broker = broker;
    }
}


void Consumer::loop() {
    KeyboardHandler key_handler;

    int i = 0;

    while (!SignalHandler::is_shutdown() && key_handler.is_running()) {
        if (key_handler.is_paused()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        std::vector<uint8_t> buffer(256);
        size_t received_size = 0;

        if (this->broker->recv(buffer.data(), buffer.size(), received_size)) {
            std::cout << "Message received successfully! (" << received_size << " bytes)\n";
            std::cout << "Data: ";
            
            Utils::Bytes::print_bytes(buffer, received_size);
        }

        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "[Consumer] Finished work.\n";
}
