#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "mesh.h"


class Model {
public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::shared_ptr<Shader> shader;
    std::string sourceDir;
    std::map<std::string, unsigned int> texPathsHash;

    Model(std::string path, std::shared_ptr<Shader> _shader);
    ~Model();
    void Draw();

    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type);
    unsigned int LoadTextureFromFile(std::string path);
};
#endif // MODEL_H