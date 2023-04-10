#ifndef NGE_MOUSE_BUTTON_EVENT_H
#define NGE_MOUSE_BUTTON_EVENT_H


#include "event.hpp"
#include "../../enums/mouse_button.h"

class MouseButtonEvent : public Event
{
public:
    MouseButtonEvent(float x, float y, MouseButton button, int modifiers);

    float x() const;
    float y() const;
    const MouseButton &button() const;
    int modifiers() const;

private:
    float x_;
    float y_;
    MouseButton button_;
    int modifiers_;
};


#endif //NGE_MOUSE_BUTTON_EVENT_H
