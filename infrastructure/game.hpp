#ifndef NGE_GAME_HPP
#define NGE_GAME_HPP

#include "logger.hpp"
#include "glad/gl.h"
#include "../data_objects/winmain_config.hpp"

class Game
{
public:
#ifdef _WIN32
    static void initialize(WinmainConfig winmain_config);
    static WinmainConfig winmainConfig();
#endif

    static void initialize();
    static void shutdown();
    static bool initialized();
    static void fault();

    static void openglDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *user_param);

private:
    static bool initialized_;

#ifdef _WIN32
    static WinmainConfig winmain_config_;
#endif
};


#endif //NGE_GAME_HPP
