#include <iostream>
#include <string>
#include <cstdlib>

#include "consumer.hpp"
#include <common/broker/transports/namedpipe.hpp>
#include <common/broker/broker.hpp>
#include <common/args.hpp>


int main(int argc, char* argv[]) {
    Utils::ARGS::CommandLineArgs parser(argc, argv);
    parser.add_option("--pipe", "string");

    std::string pipe_name = parser.get_string("--pipe", "my_pipe");

    PosixPipeTransport transport(pipe_name);
    MessageBroker broker(&transport);
    Consumer consumer(&broker);

    std::cout << "Receiving message...\n";
    consumer.loop();

    return 0;
}
