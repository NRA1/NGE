#ifndef NGE_WINMAIN_CONFIG_HPP
#define NGE_WINMAIN_CONFIG_HPP

#ifdef _WIN32
#include <minwindef.h>

struct WinmainConfig
{
    HINSTANCE Instance;
    HINSTANCE PrevInstance;
    PSTR CmdLine;
    int ShowCmd;
};

#endif

#ifdef __unix__
struct WinmainConfig {};
#endif

#endif //NGE_WINMAIN_CONFIG_HPP
