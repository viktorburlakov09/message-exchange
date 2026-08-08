#pragma once

#include <string>
#include <common/broker/broker.hpp>


class Consumer {
    private:
        Broker* broker;

    public:
        Consumer(Broker* broker);
        void loop();
};
