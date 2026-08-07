#ifndef CONSUMER_HPP

#define CONSUMER_HPP


#include <string>


class Producer {
    private:
        std::string name;
    
    public:
        void print();
        Producer(std::string* name);
};


#endif
    