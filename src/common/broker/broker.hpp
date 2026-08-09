#pragma once

#include <vector>
#include <queue>
#include <cstddef>
#include <cstdint>
#include "transport.hpp"
#include "message.hpp"


class Broker {
    public:
        explicit Broker(Transport* transport) : transport(transport) {}
        virtual ~Broker() = default;

        virtual bool send(const void* data, size_t size) = 0;
        virtual bool recv(void* data, size_t& received_size) = 0;

    protected:
        Transport* transport;
};


class MessageBroker : public Broker {
    public:
        explicit MessageBroker(Transport* transport, size_t buffer_size);
        ~MessageBroker() override = default;

        bool send(const void* data, size_t size) override;
        bool recv(void* data, size_t& received_size) override;
        bool send_raw(const uint8_t* data, size_t size);

        uint8_t* load_data(const void* data, size_t size, MessageHeader& header);
        uint8_t* get_data();
    
    private:
        std::vector<uint8_t> io_buffer;
        
        bool read_all(void* destination, size_t size);
};


class QueuedMessageBroker {
    public:
        QueuedMessageBroker(MessageBroker* broker, size_t max_queue_size = 10000);

        bool publish(const void* data, size_t size);
        bool flush();

    private:
        MessageBroker* broker;
        std::queue<std::vector<uint8_t>> queue;
        size_t max_queue_size;
};


namespace MessageUtils {
    MessageHeader make_header(const void* data, size_t size);
    void serialize(const void* data, size_t size, const MessageHeader& header, uint8_t* dest);
    bool parse_header(const uint8_t* data, size_t size, MessageHeader& header);
}
