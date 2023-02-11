#include "native_helpers.hpp"

#ifdef __unix__
#include <X11/X.h>

#include <cstddef>

Key native::nativeToKey(unsigned long keycode)
{
    return (Key)keycode;
}

#endif