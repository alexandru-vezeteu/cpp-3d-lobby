//
// Created by alex on 07/05/25.
//

#include "OpenGL3DModel.hpp"
#include <fstream>
#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



OpenGL3DModel::OpenGL3DModel(const string &name) : vaoObj{}, vboObj{} {
    if (!loadOBJ(name + "/" + name + ".obj")) {
        std::cerr << "Failed to load OBJ file" << std::endl;
        exit(EXIT_FAILURE);
    }
    size = verticesNormals.size()/2;
    std::cout<<name<<" loaded "<<getVerticesNormals().size()<<std::endl;
    GLenum err{};

    glGenVertexArrays(1, &vaoObj);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }
    glBindVertexArray(vaoObj);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }

    glGenBuffers(1, &vboObj);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vboObj);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }
    glBufferData(GL_ARRAY_BUFFER, verticesNormals.size() * sizeof(glm::vec3), &verticesNormals[0], GL_STATIC_DRAW);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }

    glEnableVertexAttribArray(0);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), static_cast<void *>(nullptr));
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }

    glEnableVertexAttribArray(1);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), reinterpret_cast<void *>(sizeof(glm::vec3)));
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err <<std::hex<< std::endl;
        exit(EXIT_FAILURE);
    }

    compileShaders(name);
}



bool OpenGL3DModel::loadOBJ(const string &path) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;


    FILE *file = fopen(path.c_str(), "r");
    if (file == nullptr) {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }

    while (true) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y;
            // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0],
                                 &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2],
                                 &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
            if (matches != 12 && matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                fclose(file);
                return false;
            }
            if (matches == 12) {
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[3]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[2]);
                uvIndices.push_back(uvIndex[3]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[2]);
                normalIndices.push_back(normalIndex[3]);
            }
            if (matches == 9) {
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        } else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        //unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        glm::vec2 uv;
        glm::vec3 normal = temp_normals[normalIndex - 1];

        // Put the attributes in buffers
        uvs.push_back(uv);
        verticesNormals.push_back(vertex);
        verticesNormals.push_back(normal);
    }
    fclose(file);
    return true;
}

OpenGL3DModel::~OpenGL3DModel() {
    if (vaoObj!=-1) {
        glDeleteVertexArrays(1, &vaoObj);

    }
    if (vboObj!=-1) {
        glDeleteBuffers(1, &vboObj);

    }
    if (shader!=-1) {
        glDeleteShader(shader);
    }

}

void OpenGL3DModel::compileShaders(const string &name) {
    auto textFileRead = [](const string &fn) {
        std::ifstream ifile(fn);
        string filetext;
        while (ifile.good()) {
            string line;
            std::getline(ifile, line);
            filetext.append(line + "\n");
        }
        return filetext;
    };

    // Load shaders
    const string vstext = textFileRead(name + "/" + name + ".vert");
    const string fstext = textFileRead(name + "/" + name + ".frag");
    const char *vertex_shader = vstext.c_str();
    const char *fragment_shader = fstext.c_str();

    // Compile vertex shader
    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    // Check for shader compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader program
    shader = glCreateProgram();
    glAttachShader(shader, fs);
    glAttachShader(shader, vs);
    glLinkProgram(shader);

    // Check for linking errors
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shaders
    glDeleteShader(vs);
    glDeleteShader(fs);
    auto printProgramInfoLog = [](const GLuint obj) {
        int infologLength = 0;
        int charsWritten = 0;

        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

        if (infologLength > 0) {
            char *info_log = static_cast<char *>(malloc(infologLength));
            glGetProgramInfoLog(obj, infologLength, &charsWritten, info_log);
            printf("%s\n", info_log);
            free(info_log);
        }
    };

    auto printShaderInfoLog = [](const GLuint obj) {
        int infologLength = 0;
        int charsWritten = 0;

        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

        if (infologLength > 0) {
            char *info_log = static_cast<char *>(malloc(infologLength));
            glGetShaderInfoLog(obj, infologLength, &charsWritten, info_log);
            printf("%s\n", info_log);
            free(info_log);
        }
    };
    // Print any additional info logs
    printShaderInfoLog(fs);
    printShaderInfoLog(vs);
    printProgramInfoLog(shader);
}


GLuint OpenGL3DModel::getShader() const {
    return shader;
}

GLuint OpenGL3DModel::getVBO() const {
    return vboObj;
}

GLuint OpenGL3DModel::getVAO() const {
    return vaoObj;
}

std::vector<vec3> OpenGL3DModel::getVerticesNormals() const {
    return verticesNormals;
}



OpenGL3DModel& OpenGL3DModel::operator=(OpenGL3DModel&& other) noexcept {
    if (this != &other) {
        // Free existing resources
        glDeleteVertexArrays(1, &vaoObj);
        glDeleteBuffers(1, &vboObj);
        glDeleteProgram(shader);

        // Move resources
        vaoObj = other.vaoObj;
        vboObj = other.vboObj;
        shader = other.shader;
        verticesNormals = std::move(other.verticesNormals);
        vertices = std::move(other.vertices);
        uvs = std::move(other.uvs);
        normals = std::move(other.normals);
        shaderUniforms = std::move(other.shaderUniforms);
        size = other.size;

        // Reset source object
        other.vaoObj = 0;
        other.vboObj = 0;
        other.shader = 0;
        other.size = 0;
    }
    return *this;
}


OpenGL3DModel::OpenGL3DModel(OpenGL3DModel&& other) noexcept
    : vaoObj(other.vaoObj),
      vboObj(other.vboObj),
      shader(other.shader),
      verticesNormals(std::move(other.verticesNormals)),
      vertices(std::move(other.vertices)),
      uvs(std::move(other.uvs)),
      normals(std::move(other.normals)),
      shaderUniforms(std::move(other.shaderUniforms)),
      size(other.size)
{
    other.vaoObj = 0;  // Use 0 instead of -1 for OpenGL objects
    other.vboObj = 0;
    other.shader = 0;
    other.size = 0;
}


void OpenGL3DModel::setVariable(const std::string &name,
                                const std::variant<float, int, vec2, vec3, vec4, mat3, mat4> &val) {
    shaderUniforms[name] = val;
}

void OpenGL3DModel::sendAllUniforms() {
    glUseProgram(shader);
    //https://en.cppreference.com/w/cpp/utility/variant/visit2
    for (auto &i: shaderUniforms) {
        auto &name = i.first;
        auto &val = i.second;
        std::visit([this, &name](auto &&arg) {
            this->sendVariable(name, arg);
        }, val);
    }
}
