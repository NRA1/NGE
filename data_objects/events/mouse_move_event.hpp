#ifndef NGE_MOUSE_MOVE_EVENT_HPP
#define NGE_MOUSE_MOVE_EVENT_HPP

#include "event.hpp"
#include "../../enums/event_type.hpp"

class MouseMoveEvent : public Event
{
public:
    MouseMoveEvent(float x, float y, int modifiers);

    int type() const override;

    float x() const;
    float y() const;
    int modifiers() const;

private:
    float x_;
    float y_;
    int modifiers_;
};


#endif //NGE_MOUSE_MOVE_EVENT_HPP
