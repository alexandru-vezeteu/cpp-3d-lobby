//
// Created by alex on 11/05/25.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <atomic>
#include <shared_mutex>

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
    std::shared_mutex playersMutex;

    Player player;
    vec3 cameraPos;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    std::shared_mutex playerMutex;

    string name;

    std::atomic<bool> running;

    static Client* instance;
    explicit Client(const std::string& name, int scale=1);
    void listen();

public:
    static std::unique_ptr<IRenderer> renderer;

    void run(int argc, char** argv);
    static Client& getInstance(const std::string& name, int scale=1);

    ~Client() {

    }

};



#endif //CLIENT_HPP
