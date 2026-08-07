#include <iostream>
#include <string>

#include "consumer.hpp"


int main() {
    std::string name = "2222";

    Consumer c(&name);
    c.print();

    return 0;
}
