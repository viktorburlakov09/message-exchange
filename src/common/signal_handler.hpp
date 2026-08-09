#pragma once

#include <csignal>
#include <atomic>
#include <iostream>

class SignalHandler {
    public:
        SignalHandler() {
            std::signal(SIGINT, SignalHandler::handle_signal);
            std::signal(SIGTERM, SignalHandler::handle_signal);
        }

        static bool is_shutdown() {
            return shutdown_requested.load();
        }

        static void request_shutdown() {
            shutdown_requested = true;
        }

    private:
        inline static std::atomic<bool> shutdown_requested{false};

        static void handle_signal(int signal) {
            if (signal == SIGINT || signal == SIGTERM) {
                std::cout << "\n[Signal] Caught stop signal (SIGINT/SIGTERM). Exiting...\n";
                shutdown_requested = true;
            }
        }
};
