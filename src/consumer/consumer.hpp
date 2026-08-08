#pragma once

#include <string>


class Consumer {
    private:
        std::string name;
    
    public:
        void print();
        Consumer(std::string* name);
};
