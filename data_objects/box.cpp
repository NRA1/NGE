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

void Box::expand(const Vertex &vertex)
{
    if(width_ < vertex.Position.x) width_ = vertex.Position.x;
    if(height_ < vertex.Position.y) height_ = vertex.Position.y;
    if(depth_ < vertex.Position.z) depth_ = vertex.Position.z;
}

void Box::unite(const Box &box)
{
    if(width_ < box.width_) width_ = box.width_;
    if(height_ < box.height_) width_ = box.height_;
    if(depth_ < box.depth_) depth_ = box.depth_;
}
