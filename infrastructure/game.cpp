#include "game.hpp"
#include "glad/gl.h"

bool Game::initialized_ = false;

void Game::initialize()
{
    if(initialized_)
    {
        log() - Debug < "Application already initialized";
        return;
    }

    initialized_ = true;
}

bool Game::initialized()
{
    return initialized_;
}

void Game::shutdown()
{
}

void Game::fault()
{
    Game::shutdown();
    exit(-1);
}

void Game::openglDebugMessageCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei,
                                      const GLchar *message, const void*)
{
    LogLevel level;
    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) level = Info;
    else if(severity == GL_DEBUG_SEVERITY_LOW) level = Debug;
    else level = Critical;

    std::string str_type;
    switch (type)
    {
        case GL_NO_ERROR:                      str_type = "GL_NO_ERROR"; break;
        case GL_INVALID_ENUM:                  str_type = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 str_type = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             str_type = "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                str_type = "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               str_type = "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 str_type = "GL_OUT_OF_MEMORY"; break;
        case 0x8031: /* not core */            str_type = "GL_TABLE_TOO_LARGE_EXT"; break;
        case 0x8065: /* not core */            str_type = "GL_TEXTURE_TOO_LARGE_EXT"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: str_type = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        default:
            str_type = "unknown type";
    }

    std::string str_severity;
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION: str_severity = "notification"; break;
        case GL_DEBUG_SEVERITY_LOW: str_severity = "low"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: str_severity = "medium"; break;
        case GL_DEBUG_SEVERITY_HIGH: str_severity = "high"; break;
        default: str_severity = "unknown severity";
    }

    log() - level < "OpenGL message of severity" < str_severity < "and type" < str_type + ":" < message;
    if(severity != GL_DEBUG_SEVERITY_NOTIFICATION) log() - level <<= trace;
}