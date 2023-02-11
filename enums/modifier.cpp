#include "modifier.hpp"

Modifier modifier::modifierFromKey(Key key)
{
    switch (key)
    {
        case KeyLShift:
            return LShiftModifier;
        case KeyRShift:
            return RShiftModifier;
        case KeyLControl:
            return LControlModifier;
        case KeyRControl:
            return RControlModifier;
        case KeyLAlt:
            return LAltModifier;
        case KeyRAlt:
            return RAltModifier;
        case KeyLSuper:
            return LSuperModifier;
        case KeyRSuper:
            return RSuperModifier;
        default:
            return NoModifier;
    }
}

bool modifier::isModifier(Key key)
{
    return modifierFromKey(key) != NoModifier;
}