#ifndef NGE_MOUSE_PRESS_EVENT_H
#define NGE_MOUSE_PRESS_EVENT_H


#include "mouse_button_event.h"

class MousePressEvent : public MouseButtonEvent
{
public:
    MousePressEvent(float x, float y, MouseButton button, int modifiers);
    int type() const override;
};


#endif //NGE_MOUSE_PRESS_EVENT_H
