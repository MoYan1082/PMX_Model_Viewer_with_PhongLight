#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures) {
    this->vertices = _vertices;
    this->indices = _indices;
    this->textures = _textures;

    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glGenBuffers(1, &EBO));

    GLCall(glBindVertexArray(VAO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

    // vertex Position
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
    // vertex normal
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
    // vertex texture coord
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)));

    GLCall(glBindVertexArray(0));
}

Mesh::~Mesh() {
    GLCall(glDeleteBuffers(1, &VAO));
    GLCall(glDeleteBuffers(1, &VBO));
    GLCall(glDeleteBuffers(1, &EBO));
}

void Mesh::Draw(std::shared_ptr<Shader> shader) {
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr = 0;
    unsigned int heightNr = 0;
    for (int i = 0; i < textures.size(); i++) {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));

        std::string uniformName;
        switch (textures[i].type) {
        case aiTextureType_DIFFUSE:
            uniformName = "texture_diffuse" + std::to_string(++diffuseNr);
            break;
        default:
            assert(false);
        }
        GLCall(glUniform1i(glGetUniformLocation(shader->ID, uniformName.c_str()), i));
        GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }
    GLCall(glActiveTexture(GL_TEXTURE0));

    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));
}