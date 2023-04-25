#ifndef NGE_ROTATION_HPP
#define NGE_ROTATION_HPP


#include "vec.hpp"

class Rotation
{
public:
    Rotation();
    Rotation(const Vec3 &vec);
    Rotation(float roll, float pitch, float yaw);

    float roll() const;
    void setRoll(float roll);
    float pitch() const;
    void setPitch(float pitch);
    float yaw() const;
    void setYaw(float yaw);
    Vec3 vec3() const;

    Rotation operator+(const Rotation &x) const;
    Rotation operator*(float x) const;

private:
    float roll_;
    float pitch_;
    float yaw_;

    friend class SaveManager;
};


#endif //NGE_ROTATION_HPP
