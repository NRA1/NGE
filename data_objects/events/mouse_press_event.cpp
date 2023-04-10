#include "mouse_press_event.h"
#include "../../enums/event_type.hpp"

MousePressEvent::MousePressEvent(float x, float y, MouseButton button, int modifiers)
        : MouseButtonEvent(x, y, button, modifiers)
{

}

int MousePressEvent::type() const
{
    return EventType::MousePressEventType;
}

