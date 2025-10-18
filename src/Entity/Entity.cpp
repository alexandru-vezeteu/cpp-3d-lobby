//
// Created by alex on 11/05/25.
//



#include <iostream>

#include "Entity.hpp"

Entity::Entity(const std::string name): model{name}, position{0, 0, 0}, rotation{0, 0, 0},
                                        name{name} {

    //altele
    auto vec = model.getSizes();

    float modelWidth  = vec[1] - vec[0]; // xMax - xMin
    float modelHeight = vec[3] - vec[2]; // yMax - yMin
    float modelDepth  = vec[5] - vec[4]; // zMax - zMin


    float targetSize = 5.0f; // fac ca toate modelele sa fie intr o cutie de 5x5x5


    if (modelWidth == 0.0f) modelWidth = 1.0f;
    if (modelHeight == 0.0f) modelHeight = 1.0f;
    if (modelDepth == 0.0f) modelDepth = 1.0f;

    float scaleX = targetSize / modelWidth;
    float scaleY = targetSize / modelHeight;
    float scaleZ = targetSize / modelDepth;
    float uniformScaleFactor = std::min(scaleX, std::min(scaleY, scaleZ));
    //presupunand ca modelul este incarcat "in picioare" cu fata catre user, il intorc cu spatele la user
    scale = glm::vec3(uniformScaleFactor, uniformScaleFactor, -uniformScaleFactor);

}

Entity::~Entity() = default;


void Entity::display(const glm::mat4& projectionM, const glm::mat4& viewM, const glm::vec3& viewPos) {


    //translate, rotate, scale
    glm::mat4 modelM{1};
    modelM = glm::translate(modelM, position);
    modelM = glm::rotate(modelM, rotation.x, {1, 0, 0});
    modelM = glm::rotate(modelM, -rotation.y, {0, 1, 0});
    modelM = glm::rotate(modelM, rotation.z, {0, 0, 1});
    modelM = glm::scale(modelM, scale);

    setVariable("model", modelM);
    setVariable("view", viewM);
    setVariable("projection", projectionM);
    setVariable("viewPos", viewPos);
    model.sendAllUniforms();

    for (const auto& mesh:model.meshes) {
        glBindVertexArray(mesh.VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.diffuseTex);
        setVariable("diffuseTexture", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh.normalTex);
        setVariable("normalTexture", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mesh.roughnessTex);
        setVariable("roughnessTexture", 2);

        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }


}


void Entity::setPosition(const glm::vec3 pos)
{
    position = pos;
}

void Entity::setRotation(const glm::vec3 rot)
{
    rotation = rot;
}



Entity::Entity(Entity&& other) noexcept
    : model(std::move(other.model)),
      position(other.position),
      rotation(other.rotation),
      scale(other.scale),
      name(std::move(other.name))
{}

Entity& Entity::operator=(Entity&& other) noexcept
{
    if (this != &other) {
        model = std::move(other.model);
        position = other.position;
        rotation = other.rotation;
        scale = other.scale;
        name = std::move(other.name);
    }
    return *this;
}


void Entity::setVariable(const std::string &name, const std::variant<   float, int,
                                                                        glm::vec2, glm::vec3,
                                                                        glm::vec4, glm::mat3, glm::mat4> &val)
{
    this->model.setVariable(name, val);
}
