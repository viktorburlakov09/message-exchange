#ifndef CONSUMER_HPP

#define CONSUMER_HPP


#include <string>


class Consumer {
    private:
        std::string name;
    
    public:
        void print();
        Consumer(std::string* name);
};


#endif
