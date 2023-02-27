#include "rotation.hpp"

Rotation::Rotation() : roll_(0), pitch_(0), yaw_(0)
{

}

Rotation::Rotation(float roll, float pitch, float yaw) : roll_(roll), pitch_(pitch), yaw_(yaw)
{

}

float Rotation::roll() const
{
    return roll_;
}

void Rotation::setRoll(float roll)
{
    roll_ = roll;
}

float Rotation::pitch() const
{
    return pitch_;
}

void Rotation::setPitch(float pitch)
{
    pitch_ = pitch;
}

float Rotation::yaw() const
{
    return yaw_;
}

void Rotation::setYaw(float yaw)
{
    yaw_ = yaw;
}

Rotation Rotation::operator+(const Rotation &x) const
{
    return { roll() + x.roll(), pitch() + x.pitch(), yaw() + x.yaw() };
}

Rotation Rotation::operator*(float x) const
{
    return { roll() * x, pitch() * x, yaw() * x };
}

