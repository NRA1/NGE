#include "mouse_move_event.hpp"

MouseMoveEvent::MouseMoveEvent(int x, int y, int modifiers) : x_(x), y_(y), modifiers_(modifiers)
{

}

int MouseMoveEvent::x() const
{
    return x_;
}

int MouseMoveEvent::y() const
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


