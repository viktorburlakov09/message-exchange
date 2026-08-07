#include "consumer.hpp"

#include <iostream>


Consumer::Consumer(std::string* name) {
    if (name != nullptr) {
        this->name = *name;
    }
}


void Consumer::print() {
    std::cout << this->name << "\n";
}
