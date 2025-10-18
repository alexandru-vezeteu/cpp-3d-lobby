//
// Created by alex on 07/05/25.
//

#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <fstream>
#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include "stb_image.h"

#include "OpenGL3DModel.hpp"

OpenGL3DModel::Mesh::Mesh(   GLuint vao, GLuint vbo, GLuint ebo,
                GLuint diffusetex, GLuint normaltex,
                GLuint roughnesstex, GLsizei indexcount) {
    VAO = vao;
    VBO = vbo;
    EBO = ebo;
    diffuseTex = diffusetex;
    normalTex = normaltex;
    roughnessTex = roughnesstex;
    indexCount = indexcount;
}
OpenGL3DModel::Mesh::~Mesh() {
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
    }
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (diffuseTex != 0) {
        glDeleteTextures(1, &diffuseTex);
    }
    if (normalTex != 0) {
        glDeleteTextures(1, &normalTex);
    }
    if (roughnessTex != 0) {
        glDeleteTextures(1, &roughnessTex);
    }
}
OpenGL3DModel::Mesh::Mesh(Mesh&& other) noexcept {

        VAO = other.VAO;
        EBO = other.EBO;
        VBO = other.VBO;

        diffuseTex = other.diffuseTex;
        normalTex = other.normalTex;
        roughnessTex = other.roughnessTex;

        indexCount = other.indexCount;

        other.VAO = 0;
        other.EBO = 0;
        other.VBO = 0;
        other.diffuseTex = 0;
        other.normalTex = 0;
        other.roughnessTex = 0;
        other.indexCount = 0;

}
OpenGL3DModel::Mesh& OpenGL3DModel::Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (VBO != 0) {
            glDeleteBuffers(1, &VBO);
        }
        if (EBO != 0) {
            glDeleteBuffers(1, &EBO);
        }
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
        }
        if (diffuseTex != 0) {
            glDeleteTextures(1, &diffuseTex);
        }
        if (normalTex != 0) {
            glDeleteTextures(1, &normalTex);
        }
        if (roughnessTex != 0) {
            glDeleteTextures(1, &roughnessTex);
        }


        VAO = other.VAO;
        EBO = other.EBO;
        VBO = other.VBO;

        diffuseTex = other.diffuseTex;
        normalTex = other.normalTex;
        roughnessTex = other.roughnessTex;

        indexCount = other.indexCount;

        other.VAO = 0;
        other.EBO = 0;
        other.VBO = 0;
        other.diffuseTex = 0;
        other.normalTex = 0;
        other.roughnessTex = 0;
        other.indexCount = 0;
    }
    return *this;
}


OpenGL3DModel::OpenGL3DModel(const std::string &name) {
    xMax = std::numeric_limits<float>::min();
    xMin = std::numeric_limits<float>::max();
    yMax = std::numeric_limits<float>::min();
    yMin = std::numeric_limits<float>::max();
    zMax = std::numeric_limits<float>::min();
    zMin = std::numeric_limits<float>::max();
    if (!loadModel(name)) {
        std::cerr << "Failed to load gltf file "<<name << std::endl;
        exit(EXIT_FAILURE);
    }

    compileShaders(name);
}
OpenGL3DModel::~OpenGL3DModel() {
    if (shader != 0) {
        glDeleteProgram(shader);
    }
}


bool OpenGL3DModel::processMesh(aiMesh *mesh, const aiScene *scene, const std::string &directory) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        //pos

        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        xMax = std::max(xMax, mesh->mVertices[i].x);
        xMin = std::min(xMin, mesh->mVertices[i].x);

        yMax = std::max(yMax, mesh->mVertices[i].y);
        yMin = std::min(yMin, mesh->mVertices[i].y);

        zMax = std::max(zMax, mesh->mVertices[i].z);
        zMin = std::min(zMin, mesh->mVertices[i].z);

        //texCoord
        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            //doar o culoare
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        //normal-garantat le am (daca nu sunt, sunt generate de assimp)
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);


        //tangent
        vertices.push_back(mesh->mTangents[i].x);
        vertices.push_back(mesh->mTangents[i].y);
        vertices.push_back(mesh->mTangents[i].z);


    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    GLuint diffuseTex = 0, normalTex = 0, roughTex = 0;
    aiString texPath;

    auto loadTexture = [&](const std::string &path)->unsigned int{
        static std::unordered_map<std::string, GLuint> textures;
        if (textures.find(path) != textures.end()) {
            return textures[path];
        }

        int w, h, ch;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);

        if (!data)
        {
            std::cerr << "Failed to load texture: " << path << std::endl;
            return 0;
        }

        GLenum format = GL_RGB;
        GLint internalFormat = GL_RGB;

        if (ch == 4) {
            format = GL_RGBA;
            internalFormat = GL_RGBA;
        } else if (ch == 1) {
            format = GL_RED;
            internalFormat = GL_R8;
        }



        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        std::cout << "Successfully loaded texture: " << path <<  std::endl;
        textures[path] = texID;
        return texID;
    };
    std::string path;

    auto createDefaultTexture =[](  unsigned char r,
                                    unsigned char g,
                                    unsigned char b,
                                    unsigned char a=255){
        GLuint tex;
        unsigned char data[4] = { r, g, b, a };
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return tex;
    };

    //textura colorata
    if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) == AI_SUCCESS ||
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
    {
        path = directory + "/"+std::string(texPath.C_Str());
        diffuseTex = loadTexture(path);
    }
    else
    {
        //culoare gri
        diffuseTex = createDefaultTexture(128, 128, 128);
    }

    //normal map
    if (material->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS ||
        material->GetTexture(aiTextureType_HEIGHT, 0, &texPath) == AI_SUCCESS)
    {
        path = directory + "/"+std::string(texPath.C_Str());
        normalTex = loadTexture(path);
    }
    else
    {
        //aproape un normal vec de 0,0,1 (V/255 *2 -1)-mapare din 0..255 in -1..1
        normalTex = createDefaultTexture(128, 128, 255);
    }

    // roughness/Metallic Map
    if (material->GetTexture(aiTextureType_METALNESS, 0, &texPath) == AI_SUCCESS ||
        material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &texPath) == AI_SUCCESS ||
        material->GetTexture(aiTextureType_SPECULAR, 0, &texPath) == AI_SUCCESS)
    {
        path = directory + "/"+std::string(texPath.C_Str());
        roughTex = loadTexture(path);
    }
    else
    {
        //r-occulision - 1
        //g-roughness - 1
        //b-metallic - 0 (non-metalic)
        //ORM convention

        roughTex = createDefaultTexture(255, 255, 0);
    }


    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // TexCoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Tangents
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);


    meshes.emplace_back(VAO, VBO, EBO, diffuseTex, normalTex, roughTex, static_cast<GLsizei>(indices.size()));

    return true;
}
bool OpenGL3DModel::processNode(aiNode *node, const aiScene *scene, const std::string& directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        if (!processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory)) {
            return false;
        }
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        if (!processNode(node->mChildren[i], scene, directory)) {
            return false;
        }
    }
    return true;
}
bool OpenGL3DModel::loadModel(const std::string &directory) {

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(directory+"/scene.gltf", 0
        | aiProcess_Triangulate             //sparge structurile mai complexe in triunghiuri (unele softuri de modelare folosesc alte primitive)
        | aiProcess_PreTransformVertices    //daca exista niste transformari, le aplica pe puncte
        | aiProcess_GenNormals              //daca nu exista normale mi le genereaza
        | aiProcess_CalcTangentSpace        //daca nu exista tangente mi le calculeaza/aproximeaza
        | aiProcess_JoinIdenticalVertices   // punctele duplicate sunt "eliminate"
        | aiProcess_SortByPType             //

        //| aiProcess_FlipUVs               // uneori trebuie
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
        return false;
    }

    return processNode(scene->mRootNode, scene, directory);
}

void OpenGL3DModel::compileShaders(const std::string &name) {
    auto textFileRead = [](const std::string &fn) {
        std::ifstream ifile(fn);
        std::string filetext;
        while (ifile.good()) {
            std::string line;
            std::getline(ifile, line);
            filetext.append(line + "\n");
        }
        return filetext;
    };


    const std::string vstext = textFileRead(name + "/" + name + ".vert");
    const std::string fstext = textFileRead(name + "/" + name + ".frag");
    const char *vertex_shader = vstext.c_str();
    const char *fragment_shader = fstext.c_str();


    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);


    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);


    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    shader = glCreateProgram();
    glAttachShader(shader, fs);
    glAttachShader(shader, vs);
    glLinkProgram(shader);


    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


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

    printShaderInfoLog(fs);
    printShaderInfoLog(vs);
    printProgramInfoLog(shader);
}

OpenGL3DModel &OpenGL3DModel::operator=(OpenGL3DModel &&other) noexcept {
    if (this != &other)
    {
        if (shader!=0) {
            glDeleteShader(shader);
        }
        xMin = other.xMin;
        yMin = other.yMin;
        zMin = other.zMin;
        xMax = other.xMax;
        yMax = other.yMax;
        zMax = other.zMax;

        shader = other.shader;
        other.shader = 0;

        meshes.clear();
        meshes = std::move(other.meshes);

        shaderUniforms.clear();
        shaderUniforms = std::move(other.shaderUniforms);
    }
    return *this;
}


OpenGL3DModel::OpenGL3DModel(OpenGL3DModel &&other) noexcept
    : shader(other.shader),
      shaderUniforms(std::move(other.shaderUniforms)),
      meshes(std::move(other.meshes))
{
    xMin = other.xMin;
    xMax = other.xMax;
    yMin = other.yMin;
    yMax = other.yMax;
    zMin = other.zMin;
    zMax = other.zMax;

    other.shader = 0;
}


void OpenGL3DModel::setVariable(const std::string &name,
                                const std::variant< float, int,
                                                    glm::vec2, glm::vec3,
                                                    glm::vec4, glm::mat3, glm::mat4> &val) {
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


