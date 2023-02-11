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
    void setStageViewport();
    void setViewport(Size *viewport);
    void setShaderProgram(ShaderProgram **shader_program);

private:
    Vec3 calculateEye();

    Object *target_object_;
    Size *viewport_;
    ShaderProgram **shader_program_;
};


#endif //NGE_CAMERA_HPP
