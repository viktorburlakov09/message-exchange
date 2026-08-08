#pragma once

 
#include <vector>
#include <cstddef>
#include "transport.hpp"
#include "message.hpp"


class Broker {
    protected:
        Transport* transport;

    public:
        explicit Broker(Transport* transport) : transport(transport) {};
        virtual ~Broker() = default;

        virtual bool send(const void* data, size_t size) = 0;
        virtual bool recv(void* data, size_t max_size, size_t& received_size) = 0;
};


class MessageBroker : public Broker {
    private:
        void make_header(const void* data, size_t size, MessageHeader& header);
        void make_packet(const uint8_t* data, size_t offset, size_t total_size, Packet& output);
        void make_buffer(const void* data, size_t size, MessageHeader& header, std::vector<uint8_t>& full_buffer);

        bool parse_header(const Packet& packet, MessageHeader& header, size_t max_size);
        bool verify_crc(const uint8_t* data, const MessageHeader& header);
        size_t copy_packet_payload(const Packet& packet, uint8_t* dest, size_t dest_offset, size_t max_expected_size);

    public:
        explicit MessageBroker(Transport* transport) : Broker(transport) {};
        ~MessageBroker() override = default;

        bool send(const void* data, size_t size) override;
        bool recv(void* data, size_t max_size, size_t& received_size) override;
};
