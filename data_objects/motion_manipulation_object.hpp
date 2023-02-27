#ifndef NGE_MOTION_MANIPULATION_OBJECT_HPP
#define NGE_MOTION_MANIPULATION_OBJECT_HPP


#include <optional>
#include "vec.hpp"
#include "rotation.hpp"

class MotionManipulationObject
{
public:
    const std::optional<Vec3> &velocity() const;
    void setVelocity(const std::optional<Vec3> &velocity);
    const std::optional<Vec3> &acceleration() const;
    void setAcceleration(const std::optional<Vec3> &acceleration);
    const std::optional<Rotation> &rotation() const;
    void setRotation(const std::optional<Rotation> &rotation);
    const bool &grabRotation() const;
    void revert();

private:
    explicit MotionManipulationObject(bool grab_rotation);

    std::optional<Vec3> velocity_;
    std::optional<Vec3> acceleration_;
    std::optional<Rotation> rotation_;
    bool grab_rotation_;

    friend class MotionVector;
};


#endif //NGE_MOTION_MANIPULATION_OBJECT_HPP
