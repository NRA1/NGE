#ifndef NGE_EVENT_TYPE_HPP
#define NGE_EVENT_TYPE_HPP

enum EventType
{
    KeyPressEventType = 0b00001,
    KeyReleaseEventType = 0b00010,
    MousePressEventType = 0b00100,
    MouseReleaseEventType = 0b01000
};

#endif //NGE_EVENT_TYPE_HPP
