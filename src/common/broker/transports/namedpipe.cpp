#include "namedpipe.hpp"
#include <iostream>
#include <unistd.h>


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


bool PosixPipeTransport::send(const uint8_t* data, size_t size) {
    if (this->fd == -1 || data == nullptr || size == 0) {
        return false;
    }

    ssize_t bytes_written = write(this->fd, data, size);
    return bytes_written == static_cast<ssize_t>(size);
}


bool PosixPipeTransport::recv(uint8_t* buffer, size_t max_size, size_t& out_size) {
    if (this->fd == -1 || buffer == nullptr || max_size == 0) {
        return false;
    }

    ssize_t bytes_read = read(this->fd, buffer, max_size);
    
    if (bytes_read <= 0) {
        return false;
    }

    out_size = static_cast<size_t>(bytes_read);
    return true;
}
