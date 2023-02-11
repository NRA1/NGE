#include "key_press_event.hpp"

KeyPressEvent::KeyPressEvent(int key, int modifiers, bool repeated)
        : repeated_(repeated), key_(key),
          modifiers_(modifiers)
{

}

bool KeyPressEvent::isRepeated() const
{
    return repeated_;
}

int KeyPressEvent::key() const
{
    return key_;
}

int KeyPressEvent::modifiers() const
{
    return modifiers_;
}

int KeyPressEvent::type() const
{
    return KeyPressEventType;
}

