#include "broker.hpp"

#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <common/time.hpp>
#include <common/crc.hpp>


bool MessageBroker::send(const void* data, size_t size) {
    if (!this->transport || !data || size == 0) {
        return false;
    }

    size_t total_msg_size = sizeof(MessageHeader) + size;

    MessageHeader header;
    this->make_header(data, size, header);
 
    std::vector<uint8_t> full_buffer(total_msg_size);
    this->make_buffer(data, size, header, full_buffer);


    size_t bytes_sent = 0;
    size_t max_packet_size = static_cast<size_t>(MAX_SIZE - sizeof(uint16_t));

    while (bytes_sent < total_msg_size) {
        size_t chunk_size = std::min(max_packet_size, total_msg_size - bytes_sent);

        Packet packet;
        this->make_packet(full_buffer.data(), bytes_sent, total_msg_size, packet);
        
        if (!this->transport->send(&packet)) {
            return false;
        }

        bytes_sent += chunk_size;
    }

    return true;
}

void MessageBroker::make_header(const void* data, size_t size, MessageHeader& header) {
    const uint8_t* byte_ptr = static_cast<const uint8_t*>(data);
    
    header.timestamp = Utils::Time::get_time_ms();
    header.size = static_cast<uint16_t>(size);
    header.crc = Utils::CRC::calculate_crc(byte_ptr, size);
}


void MessageBroker::make_buffer(const void* data, size_t size, MessageHeader& header, std::vector<uint8_t>& full_buffer) {
    std::memcpy(full_buffer.data(), &header, sizeof(MessageHeader));
    std::memcpy(full_buffer.data() + sizeof(MessageHeader), data, size);
}


void MessageBroker::make_packet(const uint8_t* data, size_t offset, size_t total_size, Packet& output) {
    size_t max_packet_size = static_cast<size_t>(MAX_SIZE - sizeof(uint16_t));
    size_t chunk_size = std::min(max_packet_size, total_size - offset);

    output.size = static_cast<uint16_t>(chunk_size);
    std::memcpy(output.payload, data + offset, chunk_size);
}


bool MessageBroker::recv(void* data, size_t max_size, size_t& received_size) {
    if (!this->transport || !data) {
        return false;
    }

    Packet first_packet;
    if (!this->transport->recv(&first_packet)) {
        return false;
    }

    MessageHeader header;
    if (!this->parse_header(first_packet, header, max_size)) {
        return false;
    }

    uint8_t* byte_dest = static_cast<uint8_t*>(data);
    size_t total_payload_received = 0;

    size_t payload_in_first = first_packet.size - sizeof(MessageHeader);
    std::memcpy(byte_dest, first_packet.payload + sizeof(MessageHeader), payload_in_first);
    total_payload_received = payload_in_first;

    while (total_payload_received < header.size) {
        Packet next_packet;

        if (!this->transport->recv(&next_packet)) {
            return false;
        }

        size_t bytes_copied = this->copy_packet_payload(
            next_packet, byte_dest, total_payload_received, header.size
        );
        
        total_payload_received += bytes_copied;
    }

    if (!this->verify_crc(byte_dest, header)) {
        return false;
    }

    received_size = header.size;
    
    return true;
}


bool MessageBroker::parse_header(const Packet& packet, MessageHeader& header, size_t max_size) {
    if (packet.size < sizeof(MessageHeader)) {
        return false;
    }

    std::memcpy(&header, packet.payload, sizeof(MessageHeader));

    if (header.size > max_size) {
        return false;
    }

    return true;
}


size_t MessageBroker::copy_packet_payload(const Packet& packet, uint8_t* dest, size_t dest_offset, size_t max_expected_size) {
    size_t bytes_to_copy = std::min(static_cast<size_t>(packet.size), max_expected_size - dest_offset);
    
    std::memcpy(dest + dest_offset, packet.payload, bytes_to_copy);
    return bytes_to_copy;
}

bool MessageBroker::verify_crc(const uint8_t* data, const MessageHeader& header) {
    uint8_t calculated_crc = Utils::CRC::calculate_crc(data, header.size);
    return calculated_crc == header.crc;
}
