#ifndef NGE_BOX_HPP
#define NGE_BOX_HPP


#include "vertex.hpp"

class Box
{
public:
    Box() = default;
    Box(float width, float height, float depth);

    float width() const;
    void setWidth(float width);
    float height() const;
    void setHeight(float height);
    float depth() const;
    void setDepth(float depth);

private:
    float width_;
    float height_;
    float depth_;
};


#endif //NGE_BOX_HPP
