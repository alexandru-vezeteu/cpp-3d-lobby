//
// Created by alex on 13/05/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Entity/Entity.hpp"
#include "../Interfaces/IControl.hpp"

class Player final : public Entity, public IControl
{

    static const glm::vec3 cameraOffset;

    glm::vec3 viewPos;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

public:
    explicit Player(std::string _name);


    ~Player() override;

    void updateCamera();

    bool mouse(int x, int y) override;

    bool specialKey(int key, int x, int y) override;

    bool control(unsigned char key, int, int) override;

    const glm::mat4 &getView() const;

    const glm::mat4 &getProjection() const;

    glm::mat4 &getProjection();

    const glm::vec3 &getPosition() const;

    const glm::vec3 &getRotation() const;

    const glm::vec3 &getViewPos() const;

};


#endif //PLAYER_HPP
