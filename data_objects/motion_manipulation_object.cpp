#include "motion_manipulation_object.hpp"

MotionManipulationObject::MotionManipulationObject(bool grab_rotation) : grab_rotation_(grab_rotation)
{
}

const std::optional<Vec3> &MotionManipulationObject::velocity() const
{
    return velocity_;
}

void MotionManipulationObject::setVelocity(const std::optional<Vec3> &velocity)
{
    velocity_ = velocity;
}

const std::optional<Vec3> &MotionManipulationObject::acceleration() const
{
    return acceleration_;
}

void MotionManipulationObject::setAcceleration(const std::optional<Vec3> &acceleration)
{
    acceleration_ = acceleration;
}

void MotionManipulationObject::revert()
{
    velocity_ = std::nullopt;
    acceleration_ = std::nullopt;
}

const bool &MotionManipulationObject::grabRotation() const
{
    return grab_rotation_;
}

const std::optional<Rotation> &MotionManipulationObject::rotation() const
{
    return rotation_;
}

void MotionManipulationObject::setRotation(const std::optional<Rotation> &rotation)
{
    rotation_ = rotation;
}

