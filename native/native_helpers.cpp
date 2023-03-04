#include "native_helpers.hpp"

#ifdef __unix__
#include <X11/X.h>

#include <cstddef>

Key native::nativeToKey(unsigned long keycode)
{
    return (Key)keycode;
}

#endif

#ifdef WIN32
Key native::nativeToKey(WPARAM keycode)
{
    if (keycode >= 48 && keycode <= 57) return (Key)keycode;
    switch (keycode)
    {
    case 81: return Key::KeyQ;
    case 87: return Key::KeyW;
    case 69: return Key::KeyE;
    case 82: return Key::KeyR;
    case 84: return Key::KeyT;
    case 90: return Key::KeyZ;
    case 85: return Key::KeyU;
    case 73: return Key::KeyI;
    case 79: return Key::KeyO;
    case 80: return Key::KeyP;
    case 65: return Key::KeyA;
    case 83: return Key::KeyS;
    case 68: return Key::KeyD;
    case 70: return Key::KeyF;
    case 71: return Key::KeyG;
    case 72: return Key::KeyH;
    case 74: return Key::KeyJ;
    case 75: return Key::KeyK;
    case 76: return Key::KeyL;
    case 89: return Key::KeyY;
    case 88: return Key::KeyX;
    case 67: return Key::KeyC;
    case 86: return Key::KeyV;
    case 66: return Key::KeyB;
    case 78: return Key::KeyN;
    case 77: return Key::KeyM;
    case 27: return Key::KeyEscape;
    case 112: return Key::KeyF1;
    case 113: return Key::KeyF2;
    case 114: return Key::KeyF3;
    case 115: return Key::KeyF4;
    case 116: return Key::KeyF5;
    case 117: return Key::KeyF6;
    case 118: return Key::KeyF7;
    case 119: return Key::KeyF8;
    case 120: return Key::KeyF9;
    case 121: return Key::KeyF10;
    case 122: return Key::KeyL1;
    case 123: return Key::KeyL2;
    case 192: return Key::KeyCedilla;
    case 9: return Key::KeyTab;
    case 13: return Key::KeyReturn;
    case 8: return Key::KeyBackspace;
    case 45: return Key::KeyInsert;
    case 46: return Key::KeyDelete;
    case 36: return Key::KeyHome;
    case 35: return Key::KeyEnd;
    case 33: return Key::KeyPageUp;
    case 34: return Key::KeyPageDown;
    case 38: return Key::KeyUp;
    case 40: return Key::KeyDown;
    case 37: return Key::KeyLeft;
    case 39: return Key::KeyRight;
    case 32: return Key::KeySpace;
    case 16: return KeyLShift;
    case 17: return KeyLControl;
    }
    return (Key) (-1);
}

#endif