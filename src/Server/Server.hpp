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
using std::string;
using std::thread;


class Server
{
private:
    Network sock;

    ThreadSafeUnorderedMap<string, ThreadSafeQueue<Command>> playerQ;
    ThreadSafeUnorderedMap<string, bool> filesReady;

    ThreadSafeUnorderedMap<string, glm::vec3> playerPos;
    ThreadSafeUnorderedMap<string, glm::vec3> playerRot;

    ThreadSafeUnorderedMap<string, thread> threads;

    ThreadSafeQueue<Command> commands;

    ThreadSafeUnorderedMap<string, std::atomic<bool>> threadsRun;


public:
    explicit Server(int accept=10);
    ~Server() = default;
    [[noreturn]]void serve();
private:
    void serveClient(const std::string name, const std::shared_ptr<Network>& client);
    void checkQ(const std::string name, const std::shared_ptr<Network>& client);

};



#endif //SERVER_HPP
