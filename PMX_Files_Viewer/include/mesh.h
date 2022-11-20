#ifndef MESH_H
#define MESH_H
#include "common.h"
#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    aiTextureType type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    Mesh(std::vector<Vertex> _vertices, 
        std::vector<unsigned int> _indices, 
        std::vector<Texture> _textures);
    ~Mesh();

    void Draw(std::shared_ptr<Shader> shader);
};
#endif // MESH_H