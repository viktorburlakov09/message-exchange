#include <iostream>
#include <string>
#include <cstdlib>

#include "consumer.hpp"
#include <common/broker/transports/namedpipe.hpp>
#include <common/broker/broker.hpp>


int main(int argc, char* argv[]) {
    std::string name = "Consumer-Node";
    Consumer c(&name);
    c.print();

    PosixPipeTransport transport("my_pipe");
    MessageBroker broker(&transport);

    std::cout << "Receiving message...\n";
    
    char buffer[256] = {0};
    size_t received_size = 0;

    if (broker.recv(buffer, sizeof(buffer), received_size)) {
        std::cout << "Message received successfully! (" << received_size << " bytes)\n";
        std::cout << "Data: " << buffer << "\n";
    } else {
        std::cerr << "Failed to receive message.\n";
    }

    return 0;
}
