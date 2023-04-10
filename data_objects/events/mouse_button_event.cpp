#include "mouse_button_event.h"

MouseButtonEvent::MouseButtonEvent(float x, float y, MouseButton button, int modifiers)
        : x_(x), y_(y), button_(button), modifiers_(modifiers)
{
}

float MouseButtonEvent::x() const
{
    return x_;
}

float MouseButtonEvent::y() const
{
    return y_;
}

const MouseButton &MouseButtonEvent::button() const
{
    return button_;
}

int MouseButtonEvent::modifiers() const
{
    return modifiers_;
}