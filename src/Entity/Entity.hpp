//
// Created by alex on 11/05/25.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../Interfaces/IDraw.hpp"
#include "../OpenGL3DModel/OpenGL3DModel.hpp"




class Entity: public IDraw
{
protected:
    OpenGL3DModel model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::string name;


public:
    explicit Entity(std::string name);
    ~Entity() override;
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);




    void setVariable(const std::string& name,const std::variant<float, int,
                                                                glm::vec2, glm::vec3,
                                                                glm::vec4, glm::mat3, glm::mat4>& val);


    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) =delete;

    Entity(Entity&& other) noexcept;
    Entity& operator=(Entity&& other) noexcept;




    void display(const glm::mat4& projectionM, const glm::mat4& viewM, const glm::vec3& viewPos) override;
};



#endif //ENTITY_HPP
