//
// Created by alex on 13/05/25.
//

#include "Player.hpp"

#include <iostream>
#include <bits/ostream.tcc>

Player::Player(const std::string _name, int scale):Entity(_name, scale) {
    std::cout << "Player created" << std::endl;
}


void Player::control(unsigned char key, int x, int y) {

}