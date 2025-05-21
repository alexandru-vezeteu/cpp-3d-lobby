//
// Created by alex on 07/05/25.
//

#ifndef IDRAWABLE_HPP
#define IDRAWABLE_HPP
#include <glm/fwd.hpp>


class IDrawable
{
public:
    virtual void display(const glm::mat4& projectionM, const glm::mat4& viewM) = 0;
    virtual ~IDrawable() = default;
};



#endif //IDRAWABLE_HPP
