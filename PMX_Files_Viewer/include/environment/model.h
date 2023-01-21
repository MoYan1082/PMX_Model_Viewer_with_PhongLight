#pragma once
#include "utility.h"
#include "common.h"
#include "mesh.h"


class Model : public Object {
public:
    Model(std::string path) {
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

    virtual void Draw() const {
        for (int i = 0; i < meshes.size(); i++)
            meshes[i]->Draw();
    }
private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string sourceDir;
    std::map<std::string, unsigned int> texPathsHash; // ШЅжи

    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int textures = 0;

        for (int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (!(mesh->mTextureCoords[0])) {
                std::cerr << "ERROR::ASSIMP::processMesh()." << std::endl;
                exit(-1);
            }
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);            
            vertices.push_back(vertex);
        }

        for (int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial* diffuse_material = scene->mMaterials[mesh->mMaterialIndex];
        int mat_size = diffuse_material->GetTextureCount(aiTextureType_DIFFUSE);
        if (mat_size == 1) {
            aiString path;
            diffuse_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            std::string texPath = sourceDir + std::string(path.C_Str());
            if (texPathsHash.find(texPath) == texPathsHash.end())
                texPathsHash[texPath] = loadTexture(texPath.c_str());
            textures = texPathsHash[texPath];
        }
        else if (mat_size == 0) {
            // TODO
            
        }
        else {
            assert(false);
        }


        return std::make_shared<Mesh>(vertices, indices, textures);
    }
    
};