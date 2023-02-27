#include "box.hpp"

Box::Box(float width, float height, float depth) : width_(width / 2), height_(height / 2), depth_(depth / 2)
{

}

float Box::width() const
{
    return width_;
}

void Box::setWidth(float width)
{
    width_ = width;
}

float Box::height() const
{
    return height_;
}

void Box::setHeight(float height)
{
    height_ = height;
}

float Box::depth() const
{
    return depth_;
}

void Box::setDepth(float depth)
{
    depth_ = depth;
}