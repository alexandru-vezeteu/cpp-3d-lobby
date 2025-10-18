//
// Created by alex on 11/05/25.
//



#include <filesystem>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <bits/std_thread.h>

#include "../GLUTRenderer/GLUTRenderer.hpp"
#include "../Compressor/Compressor.hpp"

#include "Client.hpp"




Client::Client(const std::string& name): name{name}, running{true}, renderer{GLUTRenderer::getInstance()} {

    renderer.setDisplayFunc([this]() {
        const auto& view = this->player->getView();
        const auto& projection = this->player->getProjection();
        const auto& viewPos = this->player->getViewPos();
        this->renderer.clear();
        for (auto&[fst, snd] : otherPlayers) {
            snd.display(projection, view, viewPos);
        }
        player->display(projection, view, viewPos);
        this->renderer.swapBuffers();
    });
    renderer.setTimeout(16);
    renderer.setTimeoutFunction([this](int ms) {
        auto comms = commandQueue.get_all();
        for (auto& comm : comms) {
            switch (comm.type) {
                case CommandType::JOIN: {
                    otherPlayers.insert_or_assign(comm.from, std::move(Entity{comm.from}));
                    break;
                }
                case CommandType::LEAVE: {
                    otherPlayers.erase(comm.from);
                    break;
                }
                case CommandType::UPDATE:{
                    auto it = otherPlayers.find(comm.from);
                    if (it != otherPlayers.end()) {
                        it->second.setPosition(comm.pos.value());
                        it->second.setRotation(comm.rot.value());
                    }
                    break;
                }
            }
        }
        this->renderer.redisplay();

    });
    renderer.setKeyboardFunc([this](unsigned char key, int x, int y) {

        if (this->player->control(key, x, y)) {
            Command t{CommandType::UPDATE, this->name, this->player->getPosition(), this->player->getRotation()};
            this->sock.sendCommand(t);
            this->renderer.redisplay();
        }

    });
    renderer.setSpecialKeyboardFunc([this](int key, int x, int y) {
        if (this->player->specialKey(key, x, y)) {
            Command t{CommandType::UPDATE, this->name, this->player->getPosition(), this->player->getRotation()};
            this->sock.sendCommand(t);
            this->renderer.redisplay();
        }
    });
    renderer.setReshapeFunc([this](int w, int h) {
        this->renderer.viewport(0,0,w,h);
        if (h == 0) h = 1; // Prevent division by zero
        this->player->getProjection() = glm::perspective<float>(glm::radians(45.0f), static_cast<float>(w)/static_cast<float>(h), 0.1f, 1000.0f);
    });
    renderer.setCloseFunc([this]() {
       this->running = false;
        while (!quit) {

        }

    });
}


void Client::run(int argc, char**argv)
{
    renderer.init(argc, argv, name.c_str());
    sock.connect(Config::serverHost, Config::serverPort);
    sock.sendName(name);
    player = std::make_unique<Player>(name);

    uint8_t currPlayers{};
    sock.receiveFrom(&currPlayers, sizeof(uint8_t));
    for (int i = 0; i < static_cast<int>(currPlayers); i++) {
        std::string n = sock.receiveName();
        sock.receiveFile(n + ".tar.gz");
        decompress_folder(n + ".tar.gz", n);

        const glm::vec3 pos = sock.receiveVec3();
        const glm::vec3 rot = sock.receiveVec3();

        otherPlayers.insert(std::make_pair(n, std::move(Entity(n))));
        std::cout << n << std::endl;
        auto it = otherPlayers.find(n);


        it->second.setPosition(pos);
        it->second.setRotation(rot);
    }
    compress_folder(name, name + ".tar.gz");
    sock.sendFile(name + ".tar.gz");
    std::cout << "SENT " << name << "targz" << std::endl;
    quit = false;


    std::thread listenServer(&Client::listen, this);
    renderer.startMainLoop();
}

Client& Client::getInstance(const std::string& name) {

    static Client instance{name};
    return instance;
}

void Client::listen() {
    while (running)
    {
        if (!sock.poll(100)) {
            continue;
        }

        std::unique_lock<std::shared_mutex> lock;
        auto c = sock.receiveCommand();
        switch (c.type) {
            case CommandType::JOIN: {
                sock.receiveFile(c.from+".tar.gz");
                decompress_folder(c.from+".tar.gz", c.from);
                break;
            }
            case CommandType::UPDATE: {
                break;
            }
            case CommandType::LEAVE: {
                break;
            }
        }
        commandQueue.push(c);

    }

    const Command leave{CommandType::LEAVE, name};
    sock.sendCommand(leave);
    quit = true;
}