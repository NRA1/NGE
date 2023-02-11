#include "size.hpp"

Size::Size(int width, int height)
{
    width_ = width;
    height_ = height;
}

int Size::width() const
{
    return width_;
}

void Size::setWidth(int width)
{
    width_ = width;
}

int Size::height() const
{
    return height_;
}

void Size::setHeight(int height)
{
    height_ = height;
}

bool Size::isValid() const
{
    return (width_ >= 0) && (height_ >= 0);
}
