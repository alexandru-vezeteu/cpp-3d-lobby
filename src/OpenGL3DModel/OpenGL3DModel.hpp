//
// Created by alex on 07/05/25.
//

#ifndef OPENGL3DMODEL_HPP
#define OPENGL3DMODEL_HPP
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <variant>
#include <glm/gtc/type_ptr.hpp>
#include <type_traits>

template <typename T>
constexpr bool always_false = false;


using vec2=glm::vec2;
using vec3=glm::vec3;
using vec4=glm::vec4;
using mat4=glm::mat4;
using mat3=glm::mat3;
using string = std::string;

class OpenGL3DModel
{
protected:
    std::vector< vec3 > vertices;
    std::vector< vec2 > uvs;
    std::vector< vec3 > normals;
    std::vector< vec3 > verticesNormals;
    GLuint vaoObj, vboObj;
    GLuint shader{};

    int size;

    std::unordered_map<std::string, std::variant<float,int, vec2,vec3,vec4,mat3,mat4> > shaderUniforms;
    void compileShaders(const string& name);
    bool loadOBJ(const string& path);


    //inspiratie:
    //https://www.reddit.com/r/cpp/comments/j4gsj4/always_falset/
    //https://stackoverflow.com/questions/53945490/how-to-assert-that-a-constexpr-if-else-clause-never-happen

    template<class T>
    void sendVariable(const std::string& name, const T& value) const{

        GLuint loc = glGetUniformLocation(shader, name.c_str());
        if (loc!=-1) {
            if constexpr (std::is_same_v<T, float>) {
                glUniform1f(loc, value);
            }
            else if constexpr (std::is_same_v<T, int>) {
                glUniform1i(loc, value);
            }
            else if constexpr (std::is_same_v<T, glm::vec2>) {
                glUniform2f(loc, value.x, value.y);
            }
            else if constexpr (std::is_same_v<T, glm::vec3>) {
                glUniform3f(loc, value.x, value.y, value.z);
            }
            else if constexpr (std::is_same_v<T, glm::vec4>) {
                glUniform4f(loc, value.x, value.y, value.z, value.w);
            }
            else if constexpr (std::is_same_v<T, glm::mat3>) {
                glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
            }
            else if constexpr (std::is_same_v<T, glm::mat4>) {
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
            }
            else {
                static_assert(always_false<T>, "Unsupported type for setVariable");
            }
        }
    }
public:
    explicit OpenGL3DModel(const string& name);

    OpenGL3DModel(const OpenGL3DModel&) = delete;
    OpenGL3DModel& operator=(const OpenGL3DModel&) = delete;

    OpenGL3DModel& operator=(OpenGL3DModel&&) noexcept ;
    OpenGL3DModel(OpenGL3DModel&&) noexcept ;

    void setVariable(const std::string& name, const std::variant<float,int, vec2,vec3,vec4,mat3,mat4>& val);

    void sendAllUniforms();

    ~OpenGL3DModel();
    GLuint getVAO() const;
    GLuint getVBO() const;
    GLuint getShader() const;

    int getSize(){return size;}

    std::vector<vec3>  getVerticesNormals() const;


};




#endif //OPENGL3DMODEL_HPP
