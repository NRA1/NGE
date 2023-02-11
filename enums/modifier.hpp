#ifndef NGE_MODIFIER_HPP
#define NGE_MODIFIER_HPP

#include "key.hpp"

enum Modifier
{
    ShiftModifier = 0b000000011,
    LShiftModifier = 0b00000001, // 65505,
    RShiftModifier = 0b00000010, // 65506,
    ControlModifier = 0b00001100,
    LControlModifier = 0b00000100, // 65507,
    RControlModifier = 0b001000, // 65508,
    AltModifier = 0b00110000,
    LAltModifier = 0b00010000, // 65513,
    RAltModifier = 0b00100000, // 65514,
    SuperModifier = 0b11000000,
    LSuperModifier = 0b01000000, // 65515,
    RSuperModifier = 0b10000000, // 65516,

    NoModifier = 0b00000000
};

namespace modifier
{
    Modifier modifierFromKey(Key key);
    bool isModifier(Key key);
}

#endif //NGE_MODIFIER_HPP
