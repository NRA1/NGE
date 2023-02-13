#include "rect.hpp"

Rect::Rect(float x, float y, float width, float height)
{
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
}

float Rect::x() const
{
    return x_;
}

void Rect::setX(float x)
{
    x_ = x;
}

float Rect::y() const
{
    return y_;
}

void Rect::setY(float y)
{
    y_ = y;
}

float Rect::width() const
{
    return width_;
}

void Rect::setWidth(float width)
{
    width_ = width;
}

float Rect::height() const
{
    return height_;
}

void Rect::setHeight(float height)
{
    height_ = height;
}

