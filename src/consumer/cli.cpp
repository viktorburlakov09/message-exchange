#include <iostream>
#include <string>
#include <cstdlib>

#include "consumer.hpp"
#include <common/broker/transports/namedpipe.hpp>
#include <common/broker/broker.hpp>
#include <common/args.hpp>
#include <common/signal_handler.hpp>
#include <bits/chrono.h>


int main(int argc, char* argv[]) {
    SignalHandler sig_handler;

    Utils::ARGS::CommandLineArgs parser(argc, argv);
    parser.add_option("--pipe", "string");

    std::string pipe_name = parser.get_string("--pipe", "my_pipe");

    PosixPipeTransport transport(pipe_name, O_RDONLY);
    MessageBroker broker(&transport, 256 * 1024);
    Consumer consumer(&broker);

    std::cout << "Receiving message...\n";
    consumer.loop();

    return 0;
}
