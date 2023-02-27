#include "camera.hpp"

Camera::Camera() : position_(Vec3(0, 0, 0)), rotation_(Rotation()), target_object_(nullptr), viewport_(nullptr)
{

}

void Camera::setStageViewport(ShaderProgram *shader_program)
{
    if(!viewport_->isValid())
    {
        log() - Critical < "Viewport is invalid";
        return;
    }

    Mat4 projection = glm::perspective(glm::radians(90.0f), (float)viewport_->width() / (float)viewport_->height(), 0.1f, 2000.0f);
    Mat4 view;
    if(target_object_ != nullptr)
        view = glm::lookAt(calculateEye(), target_object_->position(), Vec3(0.0f, 1.0f, 0.0f));
    else
    {
        view = Mat4(1);
        view = glm::rotate(view, glm::radians(rotation_.roll()), Vec3(0, 1, 0));
        view = glm::rotate(view, glm::radians(rotation_.pitch()), Vec3(0, 0, 1));
        view = glm::rotate(view, glm::radians(rotation_.yaw()), Vec3(1, 0, 0));
        view = glm::translate(view, position_);
    }
    shader_program->setMat4("projection", projection);
    shader_program->setMat4("view", view);
}

void Camera::setViewport(Size *viewport)
{
    viewport_ = viewport;
}

Vec3 Camera::calculateEye()
{
    if(target_object_ == nullptr)
    {
        log() - Critical < "Target object not set" <<= trace;
        return {0.0f, 0.0f,0.0f};
    }

    Mat4 rotation_mat = Mat4(1.0f);
    rotation_mat = glm::rotate(rotation_mat, glm::radians(target_object_->rotation().roll()), Vec3(0, 1, 0));
    Vec3 position = Vec3(rotation_mat * Vec4(position_, 1));
    position = target_object_->position() + position;
    return position;
}

void Camera::setTargetObject(Object *object)
{
    if(object == nullptr) motion_vector_ = MotionVector();
    else motion_vector_ = std::nullopt;
    target_object_ = object;
}

const Vec3 &Camera::position() const
{
    return position_;
}

void Camera::setPosition(const Vec3 &position)
{
    position_ = position;
}

const Rotation &Camera::rotation() const
{
    return rotation_;
}

void Camera::setRotation(const Rotation &rotation)
{
    rotation_ = rotation;
}

Object *Camera::targetObject() const
{
    return target_object_;
}

Size *Camera::viewport() const
{
    return viewport_;
}

std::optional<MotionVector> &Camera::motionVector()
{
    return motion_vector_;
}
