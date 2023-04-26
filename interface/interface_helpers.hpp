#ifndef NGE_INTERFACE_HELPERS_HPP
#define NGE_INTERFACE_HELPERS_HPP

#include "../data_objects/box.hpp"
#include "../data_objects/positioned_box.hpp"
#include "../data_objects/rotation.hpp"
#include "../infrastructure/logger.hpp"
#include "../graph/widget.hpp"
#include "../data_objects/size.hpp"
#include <glm/trigonometric.hpp>

#undef interface
namespace interface
{
    PositionedBox axisAlignedBoundingBox(const Vec3 &pos, const Rotation &rot, const PositionedBox &bounds);
    bool checkCollision(float x1, float x2, float y1, float y2);
}

#endif //NGE_INTERFACE_HELPERS_HPP
