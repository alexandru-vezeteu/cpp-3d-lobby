//
// Created by alex on 11/05/25.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <atomic>

#include "../Player//Player.hpp"
#include "../Config/Config.hpp"
#include "../Interfaces/IRenderer.hpp"
#include "../Network/Network.hpp"
#include "../Threadsafe/ThreadSafeQueue.hpp"


class Client
{

private:
    Network sock;
    std::unordered_map<std::string, Entity> otherPlayers;
    ThreadSafeQueue<Command> commandQueue;
    std::unique_ptr<Player> player;
    std::string name;

    std::atomic<bool> running;
    std::atomic<bool> quit;


    explicit Client(const std::string& name);
    void listen();

public:
    IRenderer& renderer;

    void run(int argc, char** argv);
    static Client& getInstance(const std::string& name);

    ~Client() = default;

};



#endif //CLIENT_HPP
