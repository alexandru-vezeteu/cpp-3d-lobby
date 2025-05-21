//
// Created by alex on 07/05/25.
//

#include "Socket.hpp"


#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <poll.h>
#include <cstring>
#include <unistd.h>
#include <memory>
#include <poll.h>


Socket::Socket(Socket &&other) noexcept : fd{other.fd}, s_addr{other.s_addr}, conn_addr(std::move(other.conn_addr)) {
    other.fd = -1;
    memset(&other.s_addr, 0, sizeof(other.s_addr));
}

Socket::Socket(const int domain, const int type, const int protocol) {
    fd = socket(domain, type, protocol);
    if (fd < 0) {
        std::cerr << "Cannot create socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    memset(&s_addr, 0, sizeof(s_addr));
}

Socket::~Socket() {
    if (fd >= 0) {
        std::cout<<"WTFFFFFFFFFFFFFFFF"<<std::endl;
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
}

void Socket::Bind(const std::string &addr, uint16_t port) {
    if (fd < 0) {
        std::cerr << "Cannot bind to address (" << addr << ", " << port << "). Invalid socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr.c_str(), &s_addr.sin_addr) != 1) {
        std::cerr << "Invalid IP address." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (bind(fd, reinterpret_cast<struct sockaddr *>(&s_addr), sizeof(s_addr)) < 0) {
        std::cerr << "Cannot bind to address (" << addr << ", " << port << ")." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::Listen(const int n) const {
    if (fd < 0) {
        std::cerr << "Cannot listen. Invalid socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(fd, n) < 0) {
        std::cerr << "Cannot listen.\n" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Socket Socket::Accept(const int options) const {
    if (fd < 0) {
        std::cerr << "Cannot accept a new connection. Invalid socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    Socket ret;
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    const int fd2 = accept4(fd,
                            reinterpret_cast<struct sockaddr *>(&client_addr),
                            &client_len,
                            options);
    if (fd2 < 0) {
        std::cerr << "Cannot accept a new connection." << std::endl;
        exit(EXIT_FAILURE);
    }
    ret.fd = fd2;
    ret.s_addr = client_addr;
    return ret;
}

void Socket::Connect(const std::string &addr, const uint16_t port) {
    if (fd < 0) {
        std::cerr << "Cannot connect to (" << addr << ", " << port << "). Invalid socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    conn_addr = std::make_unique<sockaddr_in>();
    conn_addr->sin_family = AF_INET;
    conn_addr->sin_port = htons(port);
    if (inet_pton(AF_INET, addr.c_str(), &(conn_addr->sin_addr)) != 1) {
        std::cerr << "Invalid IP address." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (connect(fd, reinterpret_cast<sockaddr *>(conn_addr.get()), sizeof(*conn_addr)) < 0) {
        std::cerr << "Cannot connect to (" << addr << ", " << port << ")." << std::endl;
        exit(EXIT_FAILURE);
    }
}

size_t Socket::ReceiveFrom(void *buff, const size_t size, const int options, sockaddr *src_addr,
                           const socklen_t *addrlen) const {

    if (fd < 0) {
        std::cerr << "Error receiving data. Invalid socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    src_addr = conn_addr ? reinterpret_cast<sockaddr *>(conn_addr.get()) : src_addr;
    socklen_t len = conn_addr ? sizeof(*conn_addr) : (addrlen ? *addrlen : 0);
    const ssize_t bytes_received = recvfrom(fd, buff, size, options, src_addr, &len);
    if (bytes_received < 0) {
        std::cout << size << " " << len << " " << buff << std::endl;
        std::cerr << "Error receiving data." << std::endl;
        exit(EXIT_FAILURE);
    }
    return static_cast<size_t>(bytes_received);
}

size_t Socket::SendTo(const void *buff, const size_t size, const int options, const sockaddr *dest_addr,
                      const socklen_t addrlen) const {
    if (fd < 0) {
        std::cerr << "Error sending data.. Invalid socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    const sockaddr *addr = conn_addr ? reinterpret_cast<const sockaddr *>(conn_addr.get()) : dest_addr;
    const socklen_t len = conn_addr ? sizeof(*conn_addr) : addrlen;
    const ssize_t bytes_sent = sendto(fd, buff, size, options, addr, len);
    if (bytes_sent < 0) {
        std::cerr << "Error sending data." << std::endl;
        exit(EXIT_FAILURE);
    }
    return static_cast<size_t>(bytes_sent);
}


void Socket::Close() {
    if (fd > 0) {
        close(fd);
        fd = -1;
        memset(&s_addr, 0, sizeof(s_addr));
    }
}

void Socket::Shutdown(const int opt) {
    if (fd > 0) {
        if (shutdown(fd, opt) != 0) {
            std::cerr << "Eroare la shutdown." << std::endl;
            exit(EXIT_FAILURE);
        }
        close(fd);
        fd = -1;
        memset(&s_addr, 0, sizeof(s_addr));
    }
}


size_t Socket::SendAll(const void *buffer, size_t size) const {
    size_t totalSent = 0;
    auto buff = static_cast<char *>(const_cast<void *>(buffer));
    while (totalSent < size) {
        ssize_t sent = SendTo(buff + totalSent, size - totalSent);
        if (sent < 0) {
            std::cerr << "Eroare la SendAll." << std::endl;
            exit(EXIT_FAILURE);
        }
        totalSent += sent;
    }
    return totalSent;
}

size_t Socket::RecvAll(void *buffer, size_t size) const {
    size_t totalReceived = 0;
    char *buff = static_cast<char *>(buffer);
    while (totalReceived < size) {
        ssize_t recvd = ReceiveFrom(buff + totalReceived, size - totalReceived);
        if (recvd < 0) {
            std::cerr << "Eroare la RecvAll." << std::endl;
            exit(EXIT_FAILURE);
        }
        totalReceived += recvd;
    }
    return totalReceived;
}

bool Socket::IsReadable(int ms) const {
    pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    int ret = poll(&pfd, 1, ms);
    return (ret > 0) && (pfd.revents & POLLIN);
}

bool Socket::IsWriteable(int ms) const {
    pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLOUT;

    int ret = poll(&pfd, 1, ms);
    return (ret > 0) && (pfd.revents & POLLOUT);
}
