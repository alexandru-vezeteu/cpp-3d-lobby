//
// Created by alex on 13/05/25.
//

#include <GL/glew.h>
#include <GL/freeglut_std.h>

#include "Player.hpp"

const glm::vec3 Player::cameraOffset = {0,10,10};
Player::Player(const std::string _name): Entity(_name),projectionMatrix{1.0f}, viewMatrix{1.0f}
{}

Player::~Player() = default;

void Player::updateCamera() {
    //quaternion pt rotatie
    glm::quat playerYawOrientation = glm::quat(glm::vec3(0.0f, -rotation.y, 0.0f));

   //rotesc offset ul
    glm::vec3 worldOffset = playerYawOrientation * cameraOffset;

    viewPos = position + worldOffset;
    glm::vec3 lookAtTarget = position + glm::vec3(0.0f, 1.0f, 0.0f);

    viewMatrix = glm::lookAt(viewPos, lookAtTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

bool Player::specialKey(int key, int x, int y) {
    float rotationSpeed = 0.05f;
    bool rotated = false;

    if (key == GLUT_KEY_LEFT) {
        rotation.y -= rotationSpeed;
        rotated = true;
    }
    if (key == GLUT_KEY_RIGHT) {
        rotation.y += rotationSpeed;
        rotated = true;
    }

    if (rotated) {
        updateCamera();
        return true;
    }
    return false;
}

bool Player::mouse(int x, int y) {
    return false;
}


bool Player::control(unsigned char key, int, int) {
    float moveSpeed = 0.1f;

    //quaternion pt rotatie
    glm::quat playerOrientationForMovement = glm::quat(glm::vec3(0.0f, -rotation.y, 0.0f));

    //rotesc vectorii "normali" de inainte si dreapta
    glm::vec3 playerForward = playerOrientationForMovement * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 playerRight = playerOrientationForMovement * glm::vec3(1.0f, 0.0f, 0.0f);


    //nu trb sa se modifice y-ul niciodata (pt a pastra impresia de plan sub playeri)
    playerForward.y = 0.0f;
    playerRight.y = 0.0f;

    // normalizez ca sa am viteza constanta
    playerForward = glm::normalize(playerForward);
    playerRight = glm::normalize(playerRight);


    bool moved = false;
    switch (key) {
        case 'w':
        case 'W':
            position += playerForward * moveSpeed;
            moved = true;
            break;
        case 's':
        case 'S':
            position -= playerForward * moveSpeed;
            moved = true;
            break;
        case 'a':
        case 'A':
            position -= playerRight * moveSpeed;
            moved = true;
            break;
        case 'd':
        case 'D':
            position += playerRight * moveSpeed;
            moved = true;
            break;
    }

    if (moved) {
        updateCamera();
    }

    return moved;
}


const glm::mat4 &Player::getView() const {
    return viewMatrix;
}

const glm::mat4 &Player::getProjection() const {
    return projectionMatrix;
}

glm::mat4 &Player::getProjection() {
    return projectionMatrix;
}

const glm::vec3 &Player::getPosition() const {
    return position;
}

const glm::vec3 &Player::getRotation() const {
    return rotation;
}

const glm::vec3 &Player::getViewPos() const {
    return viewPos;
}
