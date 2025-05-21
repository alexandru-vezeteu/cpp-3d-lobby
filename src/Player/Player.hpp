//
// Created by alex on 13/05/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../Entity/Entity.hpp"
#include "../Interfaces/IControable.hpp"

class Player: public Entity, public IControable
{
    vec3 camera;
    glm::mat4 projection;
public:
    explicit Player(const std::string name, int scale=1);
    ~Player() override {

    }
    void control(unsigned char, int, int) override;

    void addToPos(float x=0, float y=0, float z=0) {
        position.x += x;
    }
    void addToRot(float x=0, float y=0, float z=0) {
        rotation.x += x;
        rotation.y += y;
        rotation.z += z;
    }

    glm::mat4 getView();
    glm::mat4 getProjection();
    vec3 getPos() {
        return position;
    }
    vec3 getRot() {
        return rotation;
    }

};



#endif //PLAYER_HPP
