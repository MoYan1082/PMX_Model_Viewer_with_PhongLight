#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(std::string path, std::shared_ptr<Shader> _shader) {
    shader = _shader;
    sourceDir = path.substr(0, path.find_last_of('/') + 1);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        exit(-1);
    }
    // load all mesh from the scene with bfs
    std::queue<aiNode*> que_aiNodes;
    que_aiNodes.push(scene->mRootNode);
    while (!que_aiNodes.empty()) {
        aiNode* u = que_aiNodes.front();
        que_aiNodes.pop();
        for (int i = 0; i < u->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[u->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (int i = 0; i < u->mNumChildren; i++)
            que_aiNodes.push(u->mChildren[i]);
    }
}

Model::~Model() {}

void Model::Draw() {
    shader->use();
    for (int i = 0; i < meshes.size(); i++)
        meshes[i]->Draw(shader);
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0])
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* diffuse1 = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(diffuse1, aiTextureType_DIFFUSE);
    for (auto it : diffuseMaps) textures.push_back(it);

    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type) {
    std::vector<Texture> textures;
    for (int i = 0; i < material->GetTextureCount(type); i++) {
        aiString path;
        material->GetTexture(type, i, &path);
        std::string texPath = sourceDir + std::string(path.C_Str());
        if (texPathsHash.find(texPath) == texPathsHash.end())
            texPathsHash[texPath] = LoadTextureFromFile(texPath);

        Texture texture;
        texture.id = texPathsHash[texPath];
        texture.type = type;
        texture.path = texPath;

        textures.push_back(texture);
    }
    return textures;
}

unsigned int Model::LoadTextureFromFile(std::string path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            assert(false);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        exit(-1);
    }
    return textureID;
}
