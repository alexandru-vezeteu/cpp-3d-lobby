//
// Created by alex on 14/05/25.
//

#include <fstream>
#include <iostream>

#include "Network.hpp"



Network::Network(int domain, int type, int protocol):sock{domain, type, protocol}
{}


Network::Network(Network &&other) noexcept : sock(std::move(other.sock)){}



Network::Network(Socket&& other) noexcept : sock(std::move(other)) {}


void Network::sendVec3(const glm::vec3& v) {
    Float m;
    m.float_val = v.x;
    m.int_val = htonl(m.int_val);

    sock.SendAll(&m.int_val, sizeof(m.int_val));

    m.float_val = v.y;
    m.int_val = htonl(m.int_val);
    sock.SendAll(&m.int_val, sizeof(m.int_val));

    m.float_val = v.z;
    m.int_val = htonl(m.int_val);

    sock.SendAll(&m.int_val, sizeof(m.int_val));


}

glm::vec3 Network::receiveVec3() {
    glm::vec3 ret;

    Float m{};
    sock.RecvAll(&m.int_val, sizeof(m.int_val));
    m.int_val = ntohl(m.int_val);

    ret.x = m.float_val;

    sock.RecvAll(&m.int_val, sizeof(m.int_val));
    m.int_val = ntohl(m.int_val);

    ret.y = m.float_val;

    sock.RecvAll(&m.int_val, sizeof(m.int_val));
    m.int_val = ntohl(m.int_val);

    ret.z = m.float_val;

    return ret;
}

void Network::sendName(const std::string& name) {
    uint16_t size = name.size()+1; //+1 for \0
    size = htons(size);
    sock.SendAll(&size, sizeof(size));
    sock.SendAll(name.c_str(), name.size()+1);
}
std::string Network::receiveName()
{
    uint16_t size;
    char buffer[1024];

    sock.RecvAll(&size, sizeof(size));
    size = ntohs(size);

    sock.RecvAll(buffer, size);
    std::string name = std::string(buffer);
    return name;


}

void Network::connect(const std::string& addr, uint16_t port) {
    sock.Connect(addr, port);
}
void Network::bind(const std::string& addr, uint16_t port) {
    sock.Bind(addr, port);
}
void Network::listen(int n) const {
    sock.Listen(n);
}
Network Network::accept(int opt) const
{
    auto client = sock.Accept(opt);
    Network ret{std::move(client)};
    return ret;
}

size_t Network::receiveFrom(void* buffer, size_t size) {
    return sock.RecvAll(buffer, size);
}
void Network::sendTo(const void* buff, size_t size) {
    sock.SendAll(buff, size);
}


Command Network::receiveCommand() {
    uint8_t tip;
    sock.RecvAll(&tip, sizeof(tip));

    std::string name = receiveName();
    Command ret;
    ret.type = static_cast<CommandType>(tip);
    ret.from = std::move(name);
    if (ret.type == CommandType::UPDATE)
    {
        ret.pos = receiveVec3();
        ret.rot = receiveVec3();
    }
    else
    {
        ret.pos = std::nullopt;
        ret.rot = std::nullopt;
    }
    return ret;
}
void Network::sendCommand(const Command& command) {
    uint8_t tip = static_cast<uint8_t>(command.type);
    sock.SendAll(&tip, sizeof(tip));
    sendName(command.from);

    if (command.type == CommandType::UPDATE) {
        sendVec3(command.pos.value());
        sendVec3(command.rot.value());
    }

}




void Network::sendFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr<<"Cannot open file "<<filepath<<std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (file) {
        file.read(buffer, 1024*sizeof(char));
        size_t bytes_read = file.gcount();

        if (bytes_read == 0) {
            break;
        }
        uint16_t net_size = htons(static_cast<uint16_t>(bytes_read));
        sock.SendAll(&net_size, sizeof(net_size));
        sock.SendAll(buffer, bytes_read);
    }
    uint16_t net_size = 0;
    sock.SendAll(&net_size, sizeof(net_size));

    file.close();
}

void Network::receiveFile(const std::string& filepath) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr<<"Cannot open file "<<filepath<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<filepath<<std::endl;

    uint16_t net_size{};
    char buffer[1024];

    sock.RecvAll(&net_size, sizeof(net_size));
    auto size = static_cast<size_t>(ntohs(net_size));

    while (size > 0) {
        sock.RecvAll(buffer, size);
        file.write(buffer, size);
        sock.RecvAll(&net_size, sizeof(net_size));
        size = static_cast<size_t>(ntohs(net_size));
    }

    file.close();
}

bool Network::poll(int ms) const {
    return sock.IsReadable(ms);
}



