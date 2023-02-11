#include "mesh_component.hpp"

#include <utility>

MeshComponent::MeshComponent(std::string name, std::string model_path)
        : AbstractComponent(std::move(name)), model_(nullptr), bounding_box_(Box(0, 0, 0)),
          model_path_(std::move(model_path)), loaded_(false)
{

}

int MeshComponent::capabilities()
{
    return Capability::MeshRenderer;
}

const Box &MeshComponent::boundingBox() const
{
    return bounding_box_;
}

void MeshComponent::load()
{
    model_ = new Model(model_path_);

    setModelPosition();

    loaded_ = true;
}

void MeshComponent::unload()
{
    delete model_;
    model_ = nullptr;
    loaded_ = false;
}

void MeshComponent::render()
{
    setModelPosition();
    model_->render(*object()->stage()->shaderProgram());

//    float vertices[] = {
//            0.5f,  0.5f, 0.0f,  // top right
//            0.5f, -0.5f, 0.0f,  // bottom right
//            -0.5f, -0.5f, 0.0f,  // bottom left
//            -0.5f,  0.5f, 0.0f   // top left
//    };
//    unsigned int indices[] = {  // note that we start from 0
//            0, 1, 3,   // first triangle
//            1, 2, 3    // second triangle
//    };
//
//    unsigned int VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    unsigned int VBO;
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
//
//    unsigned int EBO;
//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//
//    shader_program_->use();
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
//    glEnableVertexAttribArray(0);
////    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
////    glDrawArrays(GL_TRIANGLES, 0, 3);
}

MeshComponent::~MeshComponent()
{
    delete model_;
}

void MeshComponent::objectChanged()
{
}

void MeshComponent::setModelPosition()
{
    if(!loaded_ && object() != nullptr) return;
    Mat4 model = Mat4(1.0f);
    model = glm::translate(model, object()->position());
    model = glm::scale(model, Vec3(1.0f, 1.0f, 1.0f));
    object()->stage()->shaderProgram()->setMat4("model", model);
}
