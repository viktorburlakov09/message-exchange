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
                if (!this->running) {
                    break;
                }

                if (line == "exit") {
                    running = false;
                    break;
                }

                this->paused = !this->paused;
                if (this->paused) {
                    std::cout << "[Keyboard] >>> PAUSED. Press [Enter] to resume.\n";
                } else {
                    std::cout << "[Keyboard] >>> RESUMED.\n";
                }
            }
        }

    public:
        KeyboardHandler() {
            this->input_thread = std::thread(&KeyboardHandler::listen_loop, this);
        }

        ~KeyboardHandler() {
            this->running = false;

            if (this->input_thread.joinable()) {
                this->input_thread.join();
            }
        }

        bool is_paused() const {
            return this->paused.load();
        }

        bool is_running() const {
            return this->running.load();
        }
};
