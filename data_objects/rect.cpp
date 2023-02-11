#include "rect.hpp"

Rect::Rect(int x, int y, int width, int height)
{
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
}

int Rect::x() const
{
    return x_;
}

void Rect::setX(int x)
{
    x_ = x;
}

int Rect::y() const
{
    return y_;
}

void Rect::setY(int y)
{
    y_ = y;
}

int Rect::width() const
{
    return width_;
}

void Rect::setWidth(int width)
{
    width_ = width;
}

int Rect::height() const
{
    return height_;
}

void Rect::setHeight(int height)
{
    height_ = height;
}

