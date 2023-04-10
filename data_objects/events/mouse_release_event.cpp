#include "mouse_release_event.h"
#include "../../enums/event_type.hpp"

MouseReleaseEvent::MouseReleaseEvent(float x, float y, MouseButton button, int modifiers)
        : MouseButtonEvent(x, y, button, modifiers)
{

}

int MouseReleaseEvent::type() const
{
    return EventType::MouseReleaseEventType;
}

