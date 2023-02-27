#ifndef NGE_MOUSE_MOVE_EVENT_HPP
#define NGE_MOUSE_MOVE_EVENT_HPP

#include "event.hpp"
#include "../../enums/event_type.hpp"

class MouseMoveEvent : public Event
{
public:
    MouseMoveEvent(int x, int y, int modifiers);

    int type() const override;

    int x() const;
    int y() const;
    int modifiers() const;

private:
    int x_;
    int y_;
    int modifiers_;
};


#endif //NGE_MOUSE_MOVE_EVENT_HPP
