//
// Created by alex on 07/05/25.
//

#ifndef OPENGL3DMODEL_HPP
#define OPENGL3DMODEL_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <variant>
#include <glm/gtc/type_ptr.hpp>
#include <type_traits>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


template<typename T>
constexpr bool always_false = false;



class OpenGL3DModel {
public:
    struct Mesh {
        GLuint VAO, VBO, EBO;
        GLuint diffuseTex, normalTex, roughnessTex;
        GLsizei indexCount;

        ~Mesh();

        Mesh() = default;

        Mesh(GLuint vao, GLuint vbo, GLuint ebo,
             GLuint diffusetex, GLuint normaltex,
             GLuint roughnesstex, GLsizei indexCount);

        Mesh(Mesh &&) noexcept;

        Mesh &operator=(Mesh &&) noexcept;

        Mesh(const Mesh &) = delete;

        Mesh &operator=(const Mesh &) = delete;
    };

    std::vector<Mesh> meshes;

protected:
    GLuint shader{};
    float xMin, xMax, yMin, yMax, zMin, zMax;

    std::unordered_map<std::string, std::variant<   float, int, glm::vec2,
                                                    glm::vec3, glm::vec4,
                                                    glm::mat3, glm::mat4> > shaderUniforms;

    void compileShaders(const std::string &name);

    bool loadModel(const std::string &path);

    bool processMesh(aiMesh *mesh, const aiScene *scene, const std::string &directory);

    bool processNode(aiNode *node, const aiScene *scene, const std::string &directory);


    //inspiratie:
    //https://www.reddit.com/r/cpp/comments/j4gsj4/always_falset/
    //https://stackoverflow.com/questions/53945490/how-to-assert-that-a-constexpr-if-else-clause-never-happen

    template<class T>
    void sendVariable(const std::string &name, const T &value) const {
        GLuint loc = glGetUniformLocation(shader, name.c_str());
        if (loc != -1) {
            if constexpr (std::is_same_v<T, float>) {
                glUniform1f(loc, value);
            } else if constexpr (std::is_same_v<T, int>) {
                glUniform1i(loc, value);
            } else if constexpr (std::is_same_v<T, glm::vec2>) {
                glUniform2f(loc, value.x, value.y);
            } else if constexpr (std::is_same_v<T, glm::vec3>) {
                glUniform3f(loc, value.x, value.y, value.z);
            } else if constexpr (std::is_same_v<T, glm::vec4>) {
                glUniform4f(loc, value.x, value.y, value.z, value.w);
            } else if constexpr (std::is_same_v<T, glm::mat3>) {
                glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
            } else if constexpr (std::is_same_v<T, glm::mat4>) {
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
            } else {
                static_assert(always_false<T>, "Unsupported type for setVariable");
            }
        }
    }

public:
    explicit OpenGL3DModel(const std::string &name);

    OpenGL3DModel(const OpenGL3DModel &) = delete;

    OpenGL3DModel &operator=(const OpenGL3DModel &) = delete;

    OpenGL3DModel &operator=(OpenGL3DModel &&) noexcept;

    OpenGL3DModel(OpenGL3DModel &&) noexcept;

    void setVariable(const std::string &name, const std::variant<   float, int,
                                                                    glm::vec2, glm::vec3,
                                                                    glm::vec4, glm::mat3, glm::mat4> &val);

    void sendAllUniforms();

    std::vector<float> getSizes() const {
        std::vector ret{xMin, xMax, yMin, yMax, zMin, zMax};
        return ret;
    }

    ~OpenGL3DModel();
};


#endif //OPENGL3DMODEL_HPP
