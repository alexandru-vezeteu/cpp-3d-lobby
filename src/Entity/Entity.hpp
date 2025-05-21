//
// Created by alex on 11/05/25.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "../Interfaces/IDrawable.hpp"
#include "../OpenGL3DModel/OpenGL3DModel.hpp"





class Entity: public IDrawable
{
protected:
    OpenGL3DModel model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 modelMatrix;
    std::string name;


public:
    explicit Entity(const std::string name, int scale=1);
    ~Entity() override;
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);




    void setVariable(const string& name,const std::variant<float,int, vec2,vec3,vec4,mat3,mat4>& val);


    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) =delete;

    Entity(Entity&& other) noexcept;
    Entity& operator=(Entity&& other) noexcept;




    void display(const mat4& projectionM, const mat4& viewM) override;
};



#endif //ENTITY_HPP
