#ifndef NGE_MODEL_HPP
#define NGE_MODEL_HPP

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "shader_program.hpp"
#include "vec.hpp"
#include "vertex.hpp"
#include "mesh.hpp"


class Model
{
public:
    explicit Model(const std::string &path);
    void render(ShaderProgram &shader_program);
    const PositionedBox &boundingBox() const;

    ~Model();

private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &type_name,
                                                const aiScene *scene);
    void calculateBoundingBox();

    std::vector<Mesh*> meshes_;
    std::vector<Texture*> loaded_textures_;
    std::string directory_;
    PositionedBox bounding_box_;
};

#endif //NGE_MODEL_HPP
