//
// Created by alex on 07/05/25.
//

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>

class PosixSocket final
{
protected:
    int fd;
    sockaddr_in s_addr{};
    std::unique_ptr<sockaddr_in> conn_addr;

public:
    PosixSocket(const PosixSocket&) = delete;
    PosixSocket& operator=(const PosixSocket&) = delete;

    PosixSocket(PosixSocket&& other) noexcept;

    explicit PosixSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    ~PosixSocket();
    void Bind(const std::string& addr, uint16_t port);
    void Listen(int n) const;
    PosixSocket Accept(int options = 0) const;
    void Connect(const std::string& addr, uint16_t port);
    size_t ReceiveFrom(void* buff, size_t size, int options = 0, sockaddr* src_addr = nullptr, const socklen_t* addrlen = nullptr) const;
    size_t SendTo(const void* buff, size_t size, int options = 0, const sockaddr* dest_addr = nullptr, socklen_t addrlen = 0) const;
    void Close();
    void Shutdown(int opt=SHUT_RDWR);

    bool IsReadable(int ms) const;
    bool IsWriteable(int ms) const;

    size_t SendAll(const void* buff, size_t size) const;
    size_t RecvAll(void* buff, size_t size) const;

};



#endif //SOCKET_HPP
