#include "interface_helpers.hpp"

PositionedBox interface::axisAlignedBoundingBox(const Vec3 &pos, const Rotation &rot, const PositionedBox &bounds)
{
    float a = std::abs(bounds.width() * glm::cos(glm::radians(rot.roll()))) + std::abs(bounds.depth() * glm::cos(glm::radians(90 - rot.roll())));
    float b = std::abs(bounds.width() * glm::sin(glm::radians(rot.roll()))) + std::abs(bounds.depth() * glm::sin(glm::radians(90 - rot.roll())));

    return PositionedBox(pos.x - (a / 2), pos.y + bounds.y(), pos.z - (b / 2), a, bounds.height(), b);
}


bool interface::checkCollision(float x1, float x2, float y1, float y2)
{
    if((x1 >= y1 && x1 <= y2) || (x2 >= y1 && x2 <= y2)) return true;
    if((y1 >= x1 && y1 <= x2) || (y2 >= x1 && y2 <= x2)) return true;
    return false;
}
