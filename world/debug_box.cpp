#include "debug_box.hpp"

#include <utility>
#include "glm/ext/matrix_transform.hpp"
#include "../interface/world_stage.hpp"
#include "camera.hpp"

DebugBox::DebugBox(std::string name, const PositionedBox &box) : Object(std::move(name)), box_(box)
{

}

void DebugBox::load()
{
    shader_program_ = new ShaderProgram(":/shaders/debug/vertex.vert", ":/shaders/debug/fragment.frag");
    unsigned int indices[] = {
            4, 0, 1,
            4, 5, 1,

            5, 1, 2,
            5, 6, 2,

            4, 5, 6,
            4, 7, 6,

            4, 0, 3,
            4, 7, 3,

            7, 3, 2,
            7, 6, 2,

            0, 1, 2,
            0, 3, 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    loadBox();
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void DebugBox::render()
{
    shader_program_->use();
    stage()->camera()->setStageViewport(shader_program_);

    Mat4 model = Mat4(1.0f);
    model = glm::translate(model, position());
    model = glm::scale(model, Vec3(1.0f, 1.0f, 1.0f));

    std::optional<Rotation> rot = motionVector().grabbedRotation();
    Rotation physics_rot;
    if(rot.has_value()) physics_rot = rotation() + rot.value();
    else physics_rot = rotation();

    model = glm::rotate(model, glm::radians(physics_rot.roll()), Vec3(0, 1, 0));
//    model = glm::rotate(model, glm::radians(physics_rot.pitch()), Vec3(0, 0, 1));
//    model = glm::rotate(model, glm::radians(physics_rot.yaw()), Vec3(1, 0, 0));
    shader_program_->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void DebugBox::unload()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader_program_;
}

void DebugBox::setBox(const PositionedBox &box)
{
    box_ = box;
    loadBox();
}

void DebugBox::loadBox()
{
    float vertices[] = {
            box_.x(), box_.y(), box_.z(),
            box_.x() + box_.width(), box_.y(), box_.z(),
            box_.x() + box_.width(), box_.y(), box_.z() + box_.depth(),
            box_.x(), box_.y(), box_.z() + box_.depth(),

            box_.x(), box_.y() + box_.height(), box_.z(),
            box_.x() + box_.width(), box_.y() + box_.height(), box_.z(),
            box_.x() + box_.width(), box_.y() + box_.height(), box_.z() + box_.depth(),
            box_.x(), box_.y() + box_.height(), box_.z() + box_.depth(),
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
}
