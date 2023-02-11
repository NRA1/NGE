#include "box.hpp"

Box::Box(int width, int height, int depth) : width_(width), height_(height), depth_(depth)
{

}

int Box::width() const
{
    return width_;
}

void Box::setWidth(int width)
{
    width_ = width;
}

int Box::height() const
{
    return height_;
}

void Box::setHeight(int height)
{
    height_ = height;
}

int Box::depth() const
{
    return depth_;
}

void Box::setDepth(int depth)
{
    depth_ = depth;
}
