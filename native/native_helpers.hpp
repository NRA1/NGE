#ifndef NGE_NATIVE_HELPERS_HPP
#define NGE_NATIVE_HELPERS_HPP

#ifdef WIN32
#include <Windows.h>
#endif

#include "../enums/key.hpp"

namespace native
{
#ifdef __unix__
    Key nativeToKey(unsigned long keycode);
#endif
#ifdef WIN32
    Key nativeToKey(WPARAM keycode);
#endif
}


#endif //NGE_NATIVE_HELPERS_HPP
