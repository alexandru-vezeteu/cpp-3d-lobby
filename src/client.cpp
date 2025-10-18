//
// Created by alex on 18/05/25.
//

#include <iostream>
#include <string>
#include <utility>

#include "Client/Client.hpp"

int main(int argc, char** argv) {
    std::string name;
     if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <name>" << std::endl;
         return 1;
     }

    name = argv[1];

    if (argc > 2) {
        Config::serverHost = argv[2];
    }

    if (argc > 3) {
        int aux = std::stoi(argv[3]);
        if (std::in_range<uint16_t>(aux) ) {
            Config::serverPort = static_cast<uint16_t>(aux);
        }
        else {
            std::cerr << "Invalid Server Port" << std::endl;
            return 1;
        }
    }


    std::cout<<"Client: "<<name<<std::endl;
    std::cout<<"Server Port: "<<Config::serverPort<<std::endl;
    std::cout<<"Server Host: "<<Config::serverHost<<std::endl;


    auto& c { Client::getInstance(name)};

    c.run(argc, argv);
    return 0;
}
