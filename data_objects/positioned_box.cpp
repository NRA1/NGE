#include "positioned_box.hpp"

PositionedBox::PositionedBox() : x_(std::nullopt), y_(std::nullopt), z_(std::nullopt), width_(0), height_(0), depth_(0)
{

}

PositionedBox::PositionedBox(float width, float height, float depth)
        : x_(std::nullopt), y_(std::nullopt), z_(std::nullopt), width_(width), height_(height), depth_(depth)
{

}

PositionedBox::PositionedBox(float x, float y, float z, float width, float height, float depth)
        : x_(x), y_(y), z_(z), width_(width), height_(height), depth_(depth)
{

}

float PositionedBox::x() const
{
    return x_.value_or(0);
}

void PositionedBox::setX(const std::optional<float> &x)
{
    x_ = x;
}

float PositionedBox::y() const
{
    return y_.value_or(0);
}

void PositionedBox::setY(const std::optional<float> &y)
{
    y_ = y;
}

float PositionedBox::z() const
{
    return z_.value_or(0);
}

void PositionedBox::setZ(const std::optional<float> &z)
{
    z_ = z;
}

float PositionedBox::width() const
{
    return width_;
}

void PositionedBox::setWidth(float width)
{
    width_ = width;
}

float PositionedBox::height() const
{
    return height_;
}

void PositionedBox::setHeight(float height)
{
    height_ = height;
}

float PositionedBox::depth() const
{
    return depth_;
}

void PositionedBox::setDepth(float depth)
{
    depth_ = depth;
}

void PositionedBox::expand(const Vertex &vertex)
{
    expand(vertex.Position.x, vertex.Position.y, vertex.Position.z);
}

void PositionedBox::expand(float x, float y, float z)
{
    if(!x_.has_value() || !y_.has_value() || !z_.has_value())
    {
        if (!x_.has_value()) x_ = x;
        if (!y_.has_value()) y_ = y;
        if (!z_.has_value()) z_ = z;
        return;
    }
    if(x_.value() + width_ < x)
        width_ = x - x_.value();
    else if(x_.value() > x)
    {
        width_ = width_ + (x_.value() - x);
        x_ = x;
    }
    if(y_.value() + height_ < y) height_ = y - y_.value();
    else if(y_.value() > y)
    {
        height_ = height_ + (y_.value() - y);
        y_ = y;
    }
    if(z_.value() + depth_ < z) depth_ = z - z_.value();
    else if(z_.value() > z)
    {
        depth_ = depth_ + (z_.value() - z);
        z_ = z;
    }
}

void PositionedBox::unite(const PositionedBox &box)
{
    expand(box.x(), box.y(), box.z());
    expand(box.x() + box.width(), box.y(), box.z());
    expand(box.x() + box.width(), box.y(), box.z() + box.depth());
    expand(box.x(), box.y(), box.z() + box.depth());
    expand(box.x(), box.y() + box.height(), box.z());
    expand(box.x() + box.width(), box.y() + box.height(), box.z());
    expand(box.x() + box.width(), box.y() + box.height(), box.z() + box.depth());
    expand(box.x(), box.y() + box.height(), box.z() + box.depth());
}
