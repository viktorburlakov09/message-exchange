#include "namedpipe.hpp"


PosixPipeTransport::PosixPipeTransport(const std::string& pipe_path, int flags)
    : fd(-1), pipe_path(pipe_path) 
{
    this->fd = open(pipe_path.c_str(), flags);
}


PosixPipeTransport::~PosixPipeTransport() {
    if (this->fd != -1) {
        close(this->fd);
    }
}


bool PosixPipeTransport::send(const Packet* packet) {
    if (this->fd == -1 || packet == nullptr) {
        return false;
    }

    size_t total_size = sizeof(uint16_t) + packet->size;
    ssize_t bytes_written = write(this->fd, packet, total_size);

    return bytes_written == static_cast<ssize_t>(total_size);
}


bool PosixPipeTransport::recv(Packet* packet) {
    if (this->fd == -1 || packet == nullptr) {
        return false;
    }

    ssize_t bytes_read = read(this->fd, &packet->size, sizeof(packet->size));
    
    if (bytes_read != static_cast<ssize_t>(sizeof(packet->size))) {
        return false;
    }

    if (packet->size > MAX_SIZE) {
        return false;
    }

    bytes_read = read(this->fd, packet->payload, packet->size);

    return bytes_read == static_cast<ssize_t>(packet->size);
}
