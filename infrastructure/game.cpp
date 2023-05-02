#include "game.hpp"
#include "glad/gl.h"
#include "resource_loader.hpp"

bool Game::initialized_ = false;
FT_Library Game::ft_;

#ifdef _WIN32
WinmainConfig Game::winmain_config_;
void Game::initialize(WinmainConfig winmain_config)
{
    winmain_config_ = winmain_config;
    if(initialized_)
    {
        log() - Debug < "Application already initialized";
        return;
    }

    if (FT_Init_FreeType(&ft_))
    {
        log() - Critical < "Could not init FreeType library. Faulting";
        fault();
    }

    initialized_ = true;
}

WinmainConfig Game::winmainConfig()
{
    return winmain_config_;
}

#endif

#ifdef __unix__
void Game::initialize()
{
//    time_t now = time(0);
//    tm *ltm = localtime(&now);

//    Log::setDefaultTarget(Log::Target(":/logs/log-" + std::to_string(ltm->tm_year + 1900) + "-" + std::to_string(ltm->tm_mon + 1) + "-" + std::to_string(ltm->tm_mday)));

    if(initialized_)
    {
        log() - Debug < "Application already initialized";
        return;
    }

    if(FT_Init_FreeType(&ft_))
    {
        log() - Critical < "Could not init FreeType library. Faulting";
        fault();
    }

    initialized_ = true;
}
#endif

bool Game::initialized()
{
    return initialized_;
}

void Game::shutdown()
{
    cleanup();
    exit(0);
}

void Game::fault()
{
    cleanup();
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
    if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
        log() - level <<= trace;
}

const FT_Library &Game::freeType()
{
    return ft_;
}

void Game::cleanup()
{
    ResourceLoader::clearCaches();
    FT_Done_FreeType(ft_);
    initialized_ = false;
}