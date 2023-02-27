#include "position.hpp"

Position::Position(float x, float y, float z) : x_(x), y_(y), z_(z)
{}

float Position::x() const
{
    return x_;
}

void Position::setX(float x)
{
    x_ = x;
}

float Position::y() const
{
    return y_;
}

void Position::setY(float y)
{
    y_ = y;
}

float Position::z() const
{
    return z_;
}

void Position::setZ(float z)
{
    z_ = z;
}

Vec3 Position::vec3()
{
    return Vec3(x_, y_, z_);
}
