//
// Created by alex on 18/05/25.
//

#include <iostream>
#include <string>

#include "src/Client/Client.hpp"

int main(int argc, char** argv) {
    string name;
    if (argc != 2) {
        name = "alex";
    }
    else {
        name = argv[1];
    }
    std::cout<<name<<std::endl;
    Client::renderer->init(argc, argv, name.c_str());
    auto& c { Client::getInstance(name)};

    c.run(argc, argv);

}
