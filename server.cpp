//
// Created by alex on 18/05/25.
//
#include "./src/Server/Server.hpp"
int main(int argc, char** argv) {

    Server s{};
    s.serve();
    while (true) {}
}