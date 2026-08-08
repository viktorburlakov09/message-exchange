#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>


class KeyboardHandler {
    private:
        std::atomic<bool> paused{false};
        std::atomic<bool> running{true};
        std::thread input_thread;

        void listen_loop() {
            std::string line;
            std::cout << "[Keyboard] Press [Enter] to Pause/Resume. Type 'exit' to stop.\n";

            while (running) {
                std::getline(std::cin, line);
                if (!running) break;

                if (line == "exit") {
                    running = false;
                    break;
                }

                paused = !paused;
                if (paused) {
                    std::cout << "[Keyboard] >>> PAUSED. Press [Enter] to resume.\n";
                } else {
                    std::cout << "[Keyboard] >>> RESUMED.\n";
                }
            }
        }

    public:
        KeyboardHandler() {
            input_thread = std::thread(&KeyboardHandler::listen_loop, this);
        }

        ~KeyboardHandler() {
            running = false;

            if (input_thread.joinable()) {
                input_thread.join();
            }
        }

        bool is_paused() const {
            return paused.load();
        }

        bool is_running() const {
            return running.load();
        }
};