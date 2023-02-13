#include "ground.hpp"
#include "glm/ext/matrix_transform.hpp"

Ground::Ground(Rect bounds, const std::string &diffuse, const std::string &specular, const std::string &normal,
               const std::string &height) : bounds_(bounds), shader_program_(nullptr)
{
    load(diffuse, specular, normal, height);
}

void Ground::load(const std::string &diffuse, const std::string &, const std::string &,
                  const std::string &)
{
    sprite_ = Sprite(bounds_, diffuse);
    sprite_->load();
}

void Ground::render() const
{
    Mat4 model = glm::rotate(Mat4(1.0f), glm::radians(90.0f), Vec3(1.0f, 0.0f, 0.0f));
    (*shader_program_)->setMat4("model", model);
    sprite_->render(*shader_program_);
}

void Ground::setShaderProgram(ShaderProgram **shader_program)
{
    shader_program_ = shader_program;
}

const Rect &Ground::bounds() const
{
    return bounds_;
}
