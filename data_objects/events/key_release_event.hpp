#ifndef NGE_KEY_RELEASE_EVENT_HPP
#define NGE_KEY_RELEASE_EVENT_HPP

#include "event.hpp"
#include "../../enums/event_type.hpp"

class KeyReleaseEvent : public Event
{
public:
    KeyReleaseEvent(int key, int modifiers);

    [[nodiscard]] int key() const;
    [[nodiscard]] int modifiers() const;

    int type() const override;

private:
    int key_;
    int modifiers_;
};


#endif //NGE_KEY_RELEASE_EVENT_HPP
