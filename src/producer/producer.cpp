#include "producer.hpp"

#include <iostream>


Producer::Producer(std::string* name) {
    if (name != nullptr) {
        this->name = *name;
    }
}


void Producer::print() {
    std::cout << this->name << "\n";
}
