#include "model.hpp"

Model::Model(const std::string &path)
{
    loadModel(ResourceLoader::fullPath(path).string());
    calculateBoundingBox();
}

void Model::render(ShaderProgram &shader_program)
{
    for(auto & mesh : meshes_)
        mesh->draw(shader_program);
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals
                                                   | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        log() - Critical < "Failed to load model from path" < path < "Reason:" < importer.GetErrorString();
        return;
    }
    directory_ = path.substr(0, path.find_last_of(DIRECTORY_SEPARATOR));

    processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene);
}


Mesh *Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        Vec3 vec3;
        vec3.x = mesh->mVertices[i].x;
        vec3.y = mesh->mVertices[i].y;
        vec3.z = mesh->mVertices[i].z;
        vertex.Position = vec3;

        if(mesh->HasNormals())
        {
            vec3.x = mesh->mNormals[i].x;
            vec3.y = mesh->mNormals[i].y;
            vec3.z = mesh->mNormals[i].z;
            vertex.Normal = vec3;
        }

        if(mesh->mTextureCoords[0])
        {
            Vec2 vec2;
            vec2.x = mesh->mTextureCoords[0][i].x;
            vec2.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec2;

            vec3.x = mesh->mTangents[i].x;
            vec3.y = mesh->mTangents[i].y;
            vec3.z = mesh->mTangents[i].z;
            vertex.Tangent = vec3;

            vec3.x = mesh->mBitangents[i].x;
            vec3.y = mesh->mBitangents[i].y;
            vec3.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vec3;
        }
        else vertex.TexCoords = Vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture*> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",
                                                              scene);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    std::vector<Texture*> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular",
                                                               scene);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    std::vector<Texture*> normal_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_normal",
                                                             scene);
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    std::vector<Texture*> height_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_height",
                                                             scene);
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    return new Mesh(vertices, indices, textures);
}

std::vector<Texture *>
Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &type_name, const aiScene *scene)
{
    std::vector<Texture*> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = std::string(str.C_Str());
        std::string texture_path = directory_ + '/' + path;

        bool skip = false;
        for(const auto & loaded_texture : loaded_textures_)
            if(loaded_texture->path() == texture_path)
            {
                textures.push_back(loaded_texture);
                skip = true;
                break;
            }

        if(!skip)
        {
            std::optional<Texture*> load_result = ResourceLoader::loadTexture(path, directory_, type_name, scene);
            Texture *texture = nullptr;
            if(!load_result.has_value())
            {
                log() - Critical < "Failed to load texture. Falling back to default texture";
                texture = common_fallbacks::defaultTexture(type_name);
            }
            else texture = load_result.value();
            textures.push_back(texture);
            loaded_textures_.push_back(texture);
        }
    }

    return textures;
}

const PositionedBox &Model::boundingBox() const
{
    return bounding_box_;
}

void Model::calculateBoundingBox()
{
    PositionedBox box;
    for(const auto &mesh : meshes_)
        box.unite(mesh->boundingBox());
    bounding_box_ = box;
}

Model::~Model()
{
    for (auto &mesh : meshes_)
    {
        delete mesh;
    }

    for(auto &texture : loaded_textures_)
    {
        delete texture;
    }
}