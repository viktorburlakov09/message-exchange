#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

class ControlHandler
{
    private:
        std::atomic<bool> is_paused{false};
        std::atomic<bool> running{true};
        std::thread listener_thread;

        void listen_loop() {
            std::string line;
            std::cout << "[Control] Press [Enter] to pause/resume. Type 'exit' to stop.\n";

            while (running) {
                std::getline(std::cin, line);

                if (!running) {
                    break;
                }

                if (line == "exit") {
                    running = false;
                    break;
                }

                is_paused = !is_paused;

                if (is_paused) {
                    std::cout << "[Control] >>> PAUSED. Press [Enter] to resume.\n";
                }
                else {
                    std::cout << "[Control] >>> RESUMED.\n";
                }
            }
        }

    public:
        ControlHandler() {
            listener_thread = std::thread(&ControlHandler::listen_loop, this);
        }

        ~ControlHandler() {
            running = false;

            if (listener_thread.joinable()) {
                listener_thread.join();
            }
        }

        bool paused() const {
            return is_paused.load();
        }

        bool is_running() const {
            return running.load();
        }

        void stop() {
            running = false;
        }
};
