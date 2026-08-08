#include <iostream>
#include <string>

#include "producer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <common/broker/transports/namedpipe.hpp>
#include <common/broker/broker.hpp>


int main() {
    std::string name = "333";

    PosixPipeTransport transport("my_pipe");
    MessageBroker broker(&transport);

    std::string message = "Hello from Producer";
    
    std::cout << "Sending message...\n";
    
    if (broker.send(message.c_str(), message.size() + 1)) {
        std::cout << "Message sent successfully!\n";
    } else {
        std::cerr << "Failed to send message.\n";
    }

    
    return 0;
}