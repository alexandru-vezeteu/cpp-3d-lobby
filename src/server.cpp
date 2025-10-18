//
// Created by alex on 18/05/25.
//
#include <iostream>
#include <utility>
#include "./src/Server/Server.hpp"
#include "./src/Config/Config.hpp"


int main(int argc, char** argv) {

    if (argc > 1) {
        Config::serverHost = argv[1];
    }

    if (argc > 2) {
        std::cout<<argv[2]<<"\n";
        int aux = std::stoi(argv[2]);
        if (std::in_range<uint16_t>(aux) ) {
            Config::serverPort = static_cast<uint16_t>(aux);
        }
        else {
            std::cerr << "Invalid Server Port" << std::endl;
            return 1;
        }
    }

    Server s{};
    s.serve();
    return 0;
}