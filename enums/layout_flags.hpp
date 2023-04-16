#ifndef NGE_LAYOUT_FLAGS_HPP
#define NGE_LAYOUT_FLAGS_HPP

enum LayoutFlags
{
    XRelative = 0x0001,
    YRelative = 0x0010,
    WidthRelative = 0x0100,
    HeightRelative = 0x1000,
    PositionRelative = 0x0011,
    SizeRelative = 0x1100,
    AllRelative = 0x1111
};

#endif //NGE_LAYOUT_FLAGS_HPP
