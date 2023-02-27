#ifndef NGE_MOTION_VECTOR_HPP
#define NGE_MOTION_VECTOR_HPP


#include <list>
#include "motion_manipulation_object.hpp"
#include "rotation.hpp"
#include "../infrastructure/logger.hpp"
#include <glm/trigonometric.hpp>

class MotionVector
{
public:
    MotionVector();

    MotionManipulationObject * generateManipulationObject(bool grab_rotation);
    void destroyManipulationObject(const MotionManipulationObject *object);

    Vec3 velocity();
    Vec3 acceleration();
    std::optional<Rotation> rotation();
    std::optional<Rotation> grabbedRotation() const;

    ~MotionVector();

private:
    std::list<MotionManipulationObject *> manipulation_objects_;

    friend class MotionManipulationObject;
};


#endif //NGE_MOTION_VECTOR_HPP
