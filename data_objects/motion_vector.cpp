#include <iostream>
#include "motion_vector.hpp"

MotionVector::MotionVector()
{

}

MotionManipulationObject *MotionVector::generateManipulationObject(bool grab_rotation)
{
    MotionManipulationObject *object = new MotionManipulationObject(grab_rotation);
    manipulation_objects_.push_back(object);
    return object;
}

void MotionVector::destroyManipulationObject(const MotionManipulationObject *object)
{
    std::erase_if(manipulation_objects_, [object](MotionManipulationObject *x) { return x == object; });
    delete object;
}

Vec3 MotionVector::velocity()
{
    if(manipulation_objects_.empty()) return Vec3(0, 0, 0);
    Vec3 vec = Vec3(0, 0, 0);
    for(const auto &obj : manipulation_objects_)
        if(obj->velocity() != std::nullopt)
        {
            if(!obj->grabRotation())
                vec += obj->velocity().value();
            else
                vec += Vec3(0, 0, glm::length(obj->velocity().value()));
        }
    return vec;
}

Vec3 MotionVector::acceleration()
{
    return Vec3();
}

MotionVector::~MotionVector()
{
    for(auto &object : manipulation_objects_)
    {
        delete object;
    }
}

std::optional<Rotation> MotionVector::grabbedRotation() const
{
    float roll_c = 0;
    float pitch_c = 0;
    float yaw_c = 0;
    int c = 0;
    for(const auto &obj : manipulation_objects_)
        if(obj->grabRotation() && obj->velocity().has_value())
        {
            Vec3 vel = obj->velocity().value();
            roll_c += glm::degrees(glm::atan(vel.x, vel.z));
            pitch_c += glm::degrees(glm::atan(vel.x, vel.y));
            yaw_c += glm::degrees(glm::atan(vel.y, vel.z));
            c++;
        }
    if(c == 0) return std::nullopt;
    return Rotation(roll_c / (float)c, pitch_c / (float)c, yaw_c / (float)c);
}

std::optional<Rotation> MotionVector::rotation()
{
    float roll = 0;
    float pitch = 0;
    float yaw = 0;
    int c = 0;
    for(const auto &obj : manipulation_objects_)
    {
        if(obj->rotation().has_value())
        {
            roll += obj->rotation()->roll();
            pitch += obj->rotation()->pitch();
            yaw += obj->rotation()->yaw();
            c++;
        }
    }
    if(c == 0) return std::nullopt;
    return Rotation(roll / (float)c, pitch / (float)c, yaw / (float)c);
}

