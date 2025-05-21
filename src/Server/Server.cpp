//
// Created by alex on 13/05/25.
//


#include <filesystem>
#include "../Config/Config.hpp"
#include "Server.hpp"
#include "../Command/Command.hpp"
#include <iostream>

Server::Server(const int accept) {
    sock.bind(Config::serverHost, Config::serverPort);
    sock.listen(accept);
}


[[noreturn]] void Server::serve() {
    //2 minute
    while (true) {
        Network client = sock.accept();
        // if (!client.poll(100)) {
        //     continue;
        // }
        auto name = client.receiveName();
        if (!playerQ.contains(name)) {
            filesReady.emplace(name, false);
            playerQ.emplace(name);

            std::cout << name << " connected" << std::endl;
            auto cl = std::make_shared<Network>(std::move(client));
            threadsRun.emplace(name, true);

            //threads.set(name, std::move(thread(&Server::serveClient, this,std::move(name), std::move(cl))));
            //vs???
            thread t(&Server::serveClient, this, name, cl);
            t.detach();
        }
    }
}

void Server::checkQ(const std::string name, const std::shared_ptr<Network> &client) {
    while (*threadsRun[name]) {
        while (!(playerQ[name]->empty()) && *threadsRun[name]) {
            Command command = playerQ[name]->front_and_pop();

            if (command.from == name) {
                continue;
            }
            std::cout << command.from << " " << static_cast<int>(command.type) << std::endl;
            switch (command.type) {
                case CommandType::JOIN:
                    if (*filesReady[command.from]) {
                        client->sendCommand(command);
                        string folder = "ServerData/" + command.from;
                        client->sendFile(folder + "/" + command.from + ".obj");
                        client->sendFile(folder + "/" + command.from + ".frag");
                        client->sendFile(folder + "/" + command.from + ".vert");
                        // client->sendVec3(*playerPos[name]);
                        // client->sendVec3(*playerRot[name]);
                    } else {
                        playerQ[name]->push(command);
                    }

                    break;
                case CommandType::LEAVE:
                    client->sendCommand(command);
                    break;
                case CommandType::UPDATE:
                    std::cout << "update de la " << command.from << " pt " << name << std::endl;
                    client->sendCommand(command);
                    break;
            }
        }
    }
}


void Server::serveClient(const std::string name, const std::shared_ptr<Network> &client) {
    std::filesystem::create_directory("ServerData/" + name);

    string folder;

    auto active = playerQ.keys([this](const string &key, const std::shared_ptr<ThreadSafeQueue<Command> > &val) {
        return *(this->filesReady[key]);
    });
    *playerPos[name] = vec3(0);
    *playerRot[name] = vec3(0);
    uint8_t size = active.size();
    client->sendTo(&size, sizeof(size));
    for (const auto &p: active) {
        folder = "ServerData/" + p;
        client->sendName(p);

        client->sendFile(folder + "/" + p + ".obj");
        client->sendFile(folder + "/" + p + ".vert");
        client->sendFile(folder + "/" + p + ".frag");

        client->sendVec3(*playerPos[p]);
        client->sendVec3(*playerRot[p]);
    }
    std::cout << name << " a primit restul playerilor." << std::endl;
    folder = "ServerData/" + name;
    std::cout << folder << " created" << std::endl;

    client->receiveFile(folder + "/" + name + ".obj");
    client->receiveFile(folder + "/" + name + ".vert");
    client->receiveFile(folder + "/" + name + ".frag");

    *filesReady[name] = true;

    Command t{CommandType::JOIN, name};
    playerQ.for_each([&t](auto &key, auto &val) {
        val->push(t);
    });


    std::thread queueCheck(&Server::checkQ, this, name, client);


    while (*threadsRun[name]) {
        // if (!client->poll(100 * 1000)) {
        //     break;
        // }
        Command comm = client->receiveCommand();
        std::cout << comm.from << " " << static_cast<int>(comm.type) << std::endl;
        if (comm.type == CommandType::UPDATE) {
            *playerRot[name] = comm.rot.value();
            *playerPos[name] = comm.pos.value();
            playerQ.for_each([&comm](auto &key, auto &val) {
                val->push(comm);
            });
        }
        if (comm.type == CommandType::LEAVE) {
            playerQ.for_each([&comm](auto &key, auto &val) {
                val->push(comm);
            });
            break;
        }
    }

    //*threadsRun[name] = false;
    queueCheck.join();

    playerQ.erase(name);
    playerRot.erase(name);
    playerPos.erase(name);
    threadsRun.erase(name);
    threads.erase(name);
}
