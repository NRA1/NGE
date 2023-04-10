#include "mouse_move_event.hpp"

MouseMoveEvent::MouseMoveEvent(float x, float y, int modifiers) : x_(x), y_(y), modifiers_(modifiers)
{

}

float MouseMoveEvent::x() const
{
    return x_;
}

float MouseMoveEvent::y() const
{
    return y_;
}


int MouseMoveEvent::modifiers() const
{
    return modifiers_;
}

int MouseMoveEvent::type() const
{
    return MouseMoveEventType;
}


