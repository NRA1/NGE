#ifndef NGE_CAMERA_HPP
#define NGE_CAMERA_HPP


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../data_objects/shader_program.hpp"
#include "../data_objects/size.hpp"
#include "object.hpp"

class WorldStage;

class Camera
{
public:
    Camera();

    void setTargetObject(Object *object);
    void setStageViewport(ShaderProgram *shader_program);
    void setViewport(Size *viewport);
    Object *targetObject() const;
    Size *viewport() const;
    const Vec3 &position() const;
    void setPosition(const Vec3 &position);
    const Rotation &rotation() const;
    void setRotation(const Rotation &rotation);
    std::optional<MotionVector> &motionVector();

private:
    Vec3 calculateEye();

    Vec3 position_;
    Rotation rotation_;
    std::optional<MotionVector> motion_vector_;

    Object *target_object_;
    Size *viewport_;

    friend class SaveManager;
};


#endif //NGE_CAMERA_HPP
