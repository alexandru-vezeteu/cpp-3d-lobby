//
// Created by alex on 11/05/25.
//

#include "Entity.hpp"

#include <iostream>
#include <ostream>


Entity::Entity(const std::string name, int scale): model{name}, position{0, 0, 0}, rotation{0, 0, 0},
                                                    scale{scale, scale, scale}, modelMatrix{1}, name{name} {
}

Entity::~Entity() {
}


void Entity::display(const mat4& projectionM, const mat4& viewM) {
    glBindVertexArray(model.getVAO());
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout<<model.getVAO()<<std::endl;
        std::cerr << name<< " OpenGL Error1: " <<std::hex<<err << std::endl;
    }
    std::cout<<name<<std::endl;
    //scale, translate, rotate
    mat4 modelM{1};
    modelM = glm::translate(modelM, position);



    modelM = glm::rotate(modelM, rotation.x, {1, 0, 0});
    modelM = glm::rotate(modelM, rotation.y, {0, 1, 0});
    modelM = glm::rotate(modelM, rotation.z, {0, 0, 1});

    setVariable("modelMatrix", modelM);

    mat4 mvp = projectionM * viewM * modelM;

    setVariable("modelViewProjectionMatrix", mvp);
    modelM = glm::transpose(glm::inverse(modelM));

    setVariable("normalMatrix", modelM);
    model.sendAllUniforms();


    glDrawArrays(GL_TRIANGLES, 0, model.getVerticesNormals().size() / 2);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr<<name<< model.getVerticesNormals().size()<< std::endl;
        std::cerr << name<<" OpenGL Error2: " <<std::hex<<err << std::endl;
    }
    else {
        std::cout<<name<< model.getVerticesNormals().size()<< std::endl;
    }
}


void Entity::setPosition(glm::vec3 pos) {
    position = pos;
}

void Entity::setRotation(glm::vec3 rot) {
    rotation = rot;
}



Entity::Entity(Entity&& other) noexcept
    : model(std::move(other.model)),
      position(other.position),
      rotation(other.rotation),
      scale(other.scale),
      modelMatrix(other.modelMatrix),
      name(std::move(other.name))
{}

Entity& Entity::operator=(Entity&& other) noexcept {
    if (this != &other) {
        model = std::move(other.model);
        position = other.position;
        rotation = other.rotation;
        scale = other.scale;
        modelMatrix = other.modelMatrix;
        name = std::move(other.name);
    }
    return *this;
}


void Entity::setVariable(const string &name, const std::variant<float, int, vec2, vec3, vec4, mat3, mat4> &val) {
    this->model.setVariable(name, val);
}
