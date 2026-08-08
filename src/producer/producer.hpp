#pragma once

#include <string>


class Producer {
    private:
        std::string name;
    
    public:
        void print();

        Producer(std::string* name);
};
