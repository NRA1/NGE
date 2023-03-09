#include "mesh.hpp"
#include "positioned_box.hpp"

#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices_, std::vector<Texture*> textures)
        : vertices_(std::move(vertices)), indices_(std::move(indices_)), textures_(std::move(textures)), VAO(0), VBO(0),
        EBO(0)
{
    setupMesh();
    calculateBoundingBox();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs_));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex), (void*) offsetof(Vertex, Weights_));

    glBindVertexArray(0);
}

void Mesh::calculateBoundingBox()
{
    PositionedBox box;
    for(const auto &vertex : vertices_)
        box.expand(vertex);
    bounding_box_ = box;
}

void Mesh::draw(ShaderProgram &shader_program)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for(unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures_[i]->type();

        if(name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if(name == "texture_specular") number = std::to_string(specularNr++);
        else if(name == "texture_normal") number = std::to_string(normalNr++);
        else if(name == "texture_height") number = std::to_string(heightNr++);

        shader_program.setInt(name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures_[i]->id());
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);
}


const PositionedBox &Mesh::boundingBox() const
{
    return bounding_box_;
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
