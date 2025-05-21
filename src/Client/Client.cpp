//
// Created by alex on 11/05/25.
//

#include "Client.hpp"

#include <filesystem>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>


#include <bits/std_thread.h>
#include "../GLUTRenderer/GLUTRenderer.hpp"


Client* Client::instance = nullptr;
std::unique_ptr<IRenderer> Client::renderer{std::make_unique<GLUTRenderer>()};
Client::Client(const std::string& name, const int scale): cameraPos(10, 10, 10), player{name}, viewMatrix{1}, projectionMatrix{1},
                                                          name{name}, running{true}, playerMutex{}, playersMutex{}
{
    sock.connect(Config::serverHost, Config::serverPort);

    sock.sendName(name);

    uint8_t currPlayers{};
    sock.receiveFrom(&currPlayers, sizeof(uint8_t));
    std::cout<<"Astept "<<static_cast<int>(currPlayers)<<std::endl;
    for (int i = 0; i < static_cast<int>(currPlayers); i++) {
        std::string n = sock.receiveName();
        std::filesystem::create_directory(n);
        std::cout << n << std::endl;
        sock.receiveFile(n + "/" + n + ".obj");
        sock.receiveFile(n + "/" + n + ".vert");
        sock.receiveFile(n + "/" + n + ".frag");

        glm::vec3 pos = sock.receiveVec3();
        glm::vec3 rot = sock.receiveVec3();

        otherPlayers.insert(std::make_pair(n, std::move(Entity(n))));
        auto it = otherPlayers.find(n);

        it->second.setPosition(pos);
        it->second.setRotation(rot);

    }
    sock.sendFile(name + "/" + name + ".obj");
    sock.sendFile(name + "/" + name + ".vert");
    sock.sendFile(name + "/" + name + ".frag");
}



void Client::run(int argc, char** argv)
{
    //renderer->init(argc, argv, "Demo game");

    renderer->setDisplayFunc([this]() {
        this->renderer->clear();
        std::shared_lock lock(playersMutex);

        for (auto&[fst, snd] : otherPlayers) {
            snd.display(this->projectionMatrix, this->viewMatrix);
        }
        player.display(this->projectionMatrix, this->viewMatrix);
        this->renderer->swapBuffers();
    });

    renderer->setTimeout(100);
    renderer->setTimerFunction([this](int _) {
        std::cout<<"timer"<<std::endl;
        std::cout<<_<<std::endl;
        if (!this->commandQueue.empty()) {
            auto c = this->commandQueue.front_and_pop();
            if (c.type == CommandType::JOIN) {
                std::unique_lock lock(playersMutex);
                otherPlayers.insert_or_assign(c.from, std::move(Entity{c.from}));
            }
        }
    });
    renderer->setKeyboardFunc([this](unsigned char key, int x, int y) {
        if (key=='a') {
            this->player.addToRot(25,25,0);
            Command t{CommandType::UPDATE, this->name, this->player.getPos(), this->player.getRot()};
            this->sock.sendCommand(t);
        }
        this->renderer->redisplay();
    });

    renderer->setReshapeFunc([this](int w, int h) {
        this->renderer->viewport(0,0,w,h);
        this->projectionMatrix = glm::perspective<float>(M_PI/4, static_cast<float>(w)/static_cast<float>(h), 0.1f, 1000.0f);
        this->viewMatrix = glm::lookAt(cameraPos, vec3(0), vec3(0,1,0));
    });

    std::thread listenServer(&Client::listen, this);
    renderer->startMainLoop();
    running = false;
    listenServer.join();

}

Client& Client::getInstance(const std::string& name, int scale) {

    static Client instance{name, scale};
    return instance;
}

void Client::listen() {
    while (running)
    {
        // if (!sock.poll(10*1000)) {
        //     break;
        // }

        std::unique_lock<std::shared_mutex> lock;
        auto c = sock.receiveCommand();
        switch (c.type) {
            case CommandType::JOIN: {
                std::cout << "JOIN" << c.from << std::endl;
                std::filesystem::create_directory(c.from);
                sock.receiveFile(c.from+"/"+c.from+".obj");
                sock.receiveFile(c.from+"/"+c.from+".frag");
                sock.receiveFile(c.from+"/"+c.from+".vert");

                // Create the model first to ensure files are loaded


                commandQueue.push(c);
                break;
            }
            case CommandType::UPDATE: {
                lock = std::unique_lock{playersMutex};
                auto it = otherPlayers.find(c.from);
                if (it != otherPlayers.end()) {
                    it->second.setPosition( c.pos.value() );
                    it->second.setRotation( c.rot.value() );
                    renderer->redisplay();
                }
                break;
            }
            case CommandType::LEAVE: {
                commandQueue.push(c);
                renderer->redisplay();
                break;
            }
        }


    }

    const Command leave{CommandType::LEAVE, name};
    sock.sendCommand(leave);
}







