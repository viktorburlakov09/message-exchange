#pragma once

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdint>
#include <cstring>
#include <common/broker/transport.hpp>


class PosixPipeTransport: public Transport {
    private:
        int fd;
        std::string pipe_path;

    public:
        PosixPipeTransport(const std::string& pipe_path, int flags = O_RDWR);
        ~PosixPipeTransport() override;

        bool send(const Packet* packet) override;
        bool recv(Packet* packet) override;
};
