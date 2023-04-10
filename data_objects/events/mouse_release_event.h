#ifndef NGE_MOUSE_RELEASE_EVENT_H
#define NGE_MOUSE_RELEASE_EVENT_H


#include "mouse_button_event.h"

class MouseReleaseEvent : public MouseButtonEvent
{
public:
    MouseReleaseEvent(float x, float y, MouseButton button, int modifiers);

    int type() const override;
};


#endif //NGE_MOUSE_RELEASE_EVENT_H
