//
// Created by alex on 13/05/25.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <thread>

#include "../Network/Network.hpp"
#include "../Threadsafe/ThreadSafeQueue.hpp"
#include "../Threadsafe/ThreadSafeUnorderedMap.hpp"
#include "../Command/Command.hpp"

class Server
{
private:
    Network sock;

    ThreadSafeUnorderedMap<std::string, ThreadSafeQueue<Command>> playerQ;
    ThreadSafeUnorderedMap<std::string, bool> filesReady;

    ThreadSafeUnorderedMap<std::string, glm::vec3> playerPos;
    ThreadSafeUnorderedMap<std::string, glm::vec3> playerRot;

    ThreadSafeUnorderedMap<std::string, std::thread> threads;

    ThreadSafeQueue<Command> commands;

    ThreadSafeUnorderedMap<std::string, std::atomic<bool>> threadsRun;


public:
    explicit Server(int accept=10);
    ~Server() = default;
    [[noreturn]]void serve();
private:
    void serveClient(std::string name, const std::shared_ptr<Network>& client);
    void checkQ(std::string name, const std::shared_ptr<Network>& client);

};



#endif //SERVER_HPP
