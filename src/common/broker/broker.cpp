#include "broker.hpp"

#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <common/time.hpp>
#include <common/crc.hpp>
#include <iostream>
#include <bytes.hpp>


MessageBroker::MessageBroker(Transport* transport, size_t buffer_size): Broker(transport) {
    this->io_buffer.resize(buffer_size);
}


bool MessageBroker::send(const void* data, size_t size) {
    if (!this->transport || !data || size == 0) {
        return false;
    }

    MessageHeader header = MessageUtils::make_header(data, size);
    size_t total_size = sizeof(MessageHeader) + size;

    if (total_size > this->io_buffer.size()) {
        this->io_buffer.resize(total_size);
    }

    uint8_t* buffer_data = this->load_data(data, size, header);

    return this->transport->send(buffer_data, total_size);
}


bool MessageBroker::send_raw(const uint8_t* data, size_t size) {
    if (!this->transport || !data || size == 0) {
        return false;
    }
    
    return this->transport->send(data, size);
}


uint8_t* MessageBroker::load_data(const void* data, size_t size, MessageHeader& header) {
    MessageUtils::serialize(data, size, header, this->io_buffer.data());
    return this->io_buffer.data();
}

uint8_t* MessageBroker::get_data() {
    return this->io_buffer.data();
}


bool MessageBroker::recv(void* data, size_t& received_size) {
    if (!this->transport || !data) {
        return false;
    }

    MessageHeader header;

    if (!this->read_all(&header, sizeof(MessageHeader))) {
        return false;
    }

    if (header.size > this->io_buffer.size()) {
        return false;
    }

    uint8_t* byte_dest = static_cast<uint8_t*>(data);
    if (!this->read_all(byte_dest, header.size)) {
        return false;
    }

    if (!Utils::CRC::verify_crc(byte_dest, header.size, header.crc)) {
        return false;
    }

    received_size = header.size;
    return true;
}


bool MessageBroker::read_all(void* destination, size_t size) {
    if (!this->transport || !destination) {
        return false;
    }

    uint8_t* byte_dest = static_cast<uint8_t*>(destination);
    size_t total_read = 0;

    while (total_read < size) {
        size_t chunk_size = size - total_read;
        size_t actual_read = 0;

        if (!this->transport->recv(byte_dest + total_read, chunk_size, actual_read) || actual_read == 0) {
            return false;
        }

        total_read += actual_read;
    }

    return true;
}


/* --- QueuedMessageBroker --- */

QueuedMessageBroker::QueuedMessageBroker(MessageBroker* broker, size_t max_queue_size)
    : broker(broker), max_queue_size(max_queue_size) {
}


bool QueuedMessageBroker::publish(const void* data, size_t size) {
    if (!data || size == 0) {
        return false;
    }

    if (this->queue.size() >= this->max_queue_size) {
        return false;
    }

    size_t total_size = sizeof(MessageHeader) + size;
    std::vector<uint8_t> message_packet(total_size);
    
    MessageHeader header = MessageUtils::make_header(data, size);
    MessageUtils::serialize(data, size, header, message_packet.data());

    this->queue.push(std::move(message_packet));
    return true;
}


bool QueuedMessageBroker::flush() {
   if (this->queue.empty()) {
        return true;
    }

    size_t total_batch_bytes = 0;
    std::queue<std::vector<uint8_t>> temp_queue = this->queue;
    
    while (!temp_queue.empty()) {
        total_batch_bytes += temp_queue.front().size();
        temp_queue.pop();
    }

    std::vector<uint8_t> batch_buffer;
    batch_buffer.reserve(total_batch_bytes);

    while (!this->queue.empty()) {
        const auto& pkt = this->queue.front();
        batch_buffer.insert(batch_buffer.end(), pkt.begin(), pkt.end());
        this->queue.pop();
    }

    return this->broker->send_raw(batch_buffer.data(), batch_buffer.size());
}


/* --- MessageUtils --- */

namespace MessageUtils {
    MessageHeader make_header(const void* data, size_t size) {
        MessageHeader header;
        const uint8_t* byte_ptr = static_cast<const uint8_t*>(data);
        
        header.timestamp = Utils::Time::get_time_ms();
        header.size = static_cast<uint16_t>(size);
        header.crc = Utils::CRC::calculate_crc(byte_ptr, size);
        
        return header;
    }

    void serialize(const void* data, size_t size, const MessageHeader& header, uint8_t* dest) {
        std::memcpy(dest, &header, sizeof(MessageHeader));
        std::memcpy(dest + sizeof(MessageHeader), data, size);
    }

    bool parse_header(const uint8_t* data, size_t size, MessageHeader& header) {
        if (size < sizeof(MessageHeader)) {
            return false;
        }

        std::memcpy(&header, data, sizeof(MessageHeader));

        return true;
    }

}
