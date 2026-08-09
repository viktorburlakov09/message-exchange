#pragma once

#include <string>
#include <common/broker/broker.hpp>


class Consumer {
    public:
        Consumer(Broker* broker);
        void loop();
    
    private:
        Broker* broker;
};
