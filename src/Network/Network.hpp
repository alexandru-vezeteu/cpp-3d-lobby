//
// Created by alex on 14/05/25.
//

#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <string>
#include <glm/vec3.hpp>

#include "../Socket/Socket.hpp"
#include "../Entity/Entity.hpp"
#include "../Command/Command.hpp"

class Network final
{
private:
    Socket sock;
    explicit Network(Socket&& other) noexcept;


    union Float {
        uint32_t int_val;
        float float_val;
    };

public:

    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;
    ~Network() = default;
    Network(Network&& other) noexcept;
    explicit Network(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);

    void bind(const std::string& addr, uint16_t port);
    void listen(int n) const;
    Network accept(int options = 0) const;
    void connect(const std::string& addr, uint16_t port);

    void receiveFile(const std::string& path);
    void sendFile(const std::string& path);
    void sendVec3(const glm::vec3& v);
    glm::vec3 receiveVec3();
    void sendName(const std::string& name);

    std::string receiveName();

    size_t receiveFrom(void* buffer, size_t size);
    void sendTo(const void* buff, size_t size);


    Command receiveCommand();
    void sendCommand(const Command& command);

    bool poll(int ms) const;

};



#endif //NETWORK_HPP
