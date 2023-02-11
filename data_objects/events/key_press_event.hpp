#ifndef NGE_KEY_PRESS_EVENT_HPP
#define NGE_KEY_PRESS_EVENT_HPP


#include "event.hpp"
#include "../../enums/event_type.hpp"

class KeyPressEvent : public Event
{
public:
    KeyPressEvent(int key, int modifiers, bool repeated);

    [[nodiscard]] bool isRepeated() const;
    [[nodiscard]] int key() const;
    [[nodiscard]] int modifiers() const;

    int type() const override;

private:
    bool repeated_;
    int key_;
    int modifiers_;
};


#endif //NGE_KEY_PRESS_EVENT_HPP
