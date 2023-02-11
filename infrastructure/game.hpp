#ifndef NGE_GAME_HPP
#define NGE_GAME_HPP

#include "logger.hpp"
#include "glad/gl.h"

class Game
{
public:
    static void initialize();
    static void shutdown();
    static bool initialized();
    static void fault();

    static void openglDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *user_param);

private:
    static bool initialized_;
};


#endif //NGE_GAME_HPP
