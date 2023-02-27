#ifndef NGE_POSITIONED_BOX_HPP
#define NGE_POSITIONED_BOX_HPP

#include <optional>
#include "box.hpp"
#include "position.hpp"

class PositionedBox
{
public:
    PositionedBox();
    PositionedBox(float width, float height, float depth);
    PositionedBox(float x, float y, float z, float width, float height, float depth);

    float x() const;
    void setX(const std::optional<float> &x);
    float y() const;
    void setY(const std::optional<float> &y);
    float z() const;
    void setZ(const std::optional<float> &z);
    float width() const;
    void setWidth(float width);
    float height() const;
    void setHeight(float height);
    float depth() const;
    void setDepth(float depth);

    void expand(const Vertex &vertex);
    void expand(float x, float y, float z);
    void unite(const PositionedBox &box);

private:
    std::optional<float> x_;
    std::optional<float> y_;
    std::optional<float> z_;
    float width_;
    float height_;
    float depth_;
};


#endif //NGE_POSITIONED_BOX_HPP
