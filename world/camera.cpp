#include "camera.hpp"

Camera::Camera() : target_object_(nullptr), viewport_(nullptr)
{

}

void Camera::setStageViewport(ShaderProgram *shader_program)
{
    if(target_object_ == nullptr)
    {
        log() - Critical < "Target object not set";
        return;
    }
    if(!viewport_->isValid())
    {
        log() - Critical < "Viewport is invalid";
        return;
    }

    Mat4 projection = glm::perspective(glm::radians(90.0f), (float)viewport_->width() / (float)viewport_->height(), 0.1f, 1000.0f);
    Mat4 view = glm::lookAt(calculateEye(), target_object_->position(), Vec3(0.0f, 1.0f, 0.0f));
//    Mat4 view = glm::lookAt(Vec3(200.0f, 200.0f, 200.0f), target_object_->position(), Vec3(0.0f, 1.0f, 0.0f));
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

    Vec3 position = target_object_->position() + Vec3(200.0f, 200.0f, 200.0f);
    return position;
}

void Camera::setTargetObject(Object *object)
{
    target_object_ = object;
}