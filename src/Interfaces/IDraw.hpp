//
// Created by alex on 07/05/25.
//

#ifndef IDRAW_HPP
#define IDRAW_HPP
#include <glm/fwd.hpp>


class IDraw
{
public:
    virtual void display(const glm::mat4& projectionM, const glm::mat4& viewM, const glm::vec3& viewPos) = 0;
    virtual ~IDraw() = default;
};



#endif //IDRAW_HPP
