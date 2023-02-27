#ifndef NGE_POSITION_HPP
#define NGE_POSITION_HPP


#include "vec.hpp"

class Position
{
public:
    Position(float x = 0, float y = 0, float z = 0);

    float x() const;
    void setX(float x);
    float y() const;
    void setY(float y);
    float z() const;
    void setZ(float z);

    Vec3 vec3();

private:
    float x_;
    float y_;
    float z_;
};


#endif //NGE_POSITION_HPP
