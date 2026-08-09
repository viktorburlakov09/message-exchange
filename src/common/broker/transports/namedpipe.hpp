#pragma once

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdint>
#include <cstring>
#include <common/broker/transport.hpp>


class PosixPipeTransport: public Transport {
    public:
        PosixPipeTransport(const std::string& pipe_path, int flags = O_RDWR);
        ~PosixPipeTransport() override;

        bool send(const uint8_t* data, size_t size) override;
        bool recv(uint8_t* buffer, size_t max_size, size_t& out_size) override;

    private:
        int fd;
        std::string pipe_path;
};
