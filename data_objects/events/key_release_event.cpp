#include "key_release_event.hpp"

KeyReleaseEvent::KeyReleaseEvent(int key, int modifiers) : key_(key), modifiers_(modifiers)
{

}

int KeyReleaseEvent::key() const
{
    return key_;
}

int KeyReleaseEvent::modifiers() const
{
    return modifiers_;
}

int KeyReleaseEvent::type() const
{
    return KeyReleaseEventType;
}
