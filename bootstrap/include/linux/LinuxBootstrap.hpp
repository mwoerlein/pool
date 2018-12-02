#ifndef LINUX_LINUXBOOTSTRAP_HPP_LOCK
#define LINUX_LINUXBOOTSTRAP_HPP_LOCK

#include "sys/Environment.hpp"
#include "sys/stream/StreamFactory.hpp"

class LinuxBootstrap {
    public:
    static Environment & buildEnvironment();
};

#endif //LINUX_LINUXBOOTSTRAP_HPP_LOCK
