#ifndef NGE_MESH_HPP
#define NGE_MESH_HPP

#include <vector>
#include "vertex.hpp"
#include "texture.hpp"
#include "shader_program.hpp"
#include "box.hpp"

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices_, std::vector<Texture*> textures);
    void draw(ShaderProgram &shader_program);
    const Box &boundingBox() const;

    ~Mesh();

private:
    void setupMesh();
    void calculateBoundingBox();

    std::vector<Vertex> vertices_;
    Box bounding_box_;
    std::vector<unsigned int> indices_;
    std::vector<Texture*> textures_;
    unsigned int VAO, VBO, EBO;
};


#endif //NGE_MESH_HPP
