#include "size.hpp"

Size::Size(float width, float height)
{
    width_ = width;
    height_ = height;
}

float Size::width() const
{
    return width_;
}

void Size::setWidth(float width)
{
    width_ = width;
}

float Size::height() const
{
    return height_;
}

void Size::setHeight(float height)
{
    height_ = height;
}

bool Size::isValid() const
{
    return (width_ >= 0) && (height_ >= 0);
}
