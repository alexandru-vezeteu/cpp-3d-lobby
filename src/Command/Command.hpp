//
// Created by alex on 18/05/25.
//

#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <optional>
#include <string>
#include <glm/vec3.hpp>
enum class CommandType {
    JOIN,
    LEAVE,
    UPDATE,
};
struct Command
{
    CommandType type;
    std::string from;
    std::optional<glm::vec3> pos;
    std::optional<glm::vec3> rot;
};
#endif //COMMAND_HPP
