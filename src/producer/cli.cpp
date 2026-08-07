#include <iostream>
#include <string>

#include "producer.hpp"


int main() {
    std::string name = "333";

    Producer p(&name);
    p.print();

    return 0;
}
