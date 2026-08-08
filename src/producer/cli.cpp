#include <iostream>
#include <string>

#include "producer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <common/broker/transports/namedpipe.hpp>
#include <common/broker/broker.hpp>
#include <common/args.hpp>
#include <common/signal_handler.hpp>


int main(int argc, char* argv[]) {
    SignalHandler sig_handler;

    Utils::ARGS::CommandLineArgs parser(argc, argv);
    parser.add_option("--pipe", "string");
    parser.add_option("--size", "size_t");

    std::string pipe_name = parser.get_string("--pipe", "my_pipe");
    size_t packet_size = parser.get_size_t("--size", 10);

    PosixPipeTransport transport(pipe_name);
    MessageBroker broker(&transport);
    Producer producer(&broker, packet_size);

    std::cout << "Sending message...\n";
    
    producer.loop();
    
    return 0;
}
