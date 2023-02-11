#ifndef NGE_ASSERTIONS_HPP
#define NGE_ASSERTIONS_HPP

#ifdef DEBUG
#define assertf(x, y) if(x != y) log << Logger::Fatal << "Assertion failed" << fault(-1);

#define assertGlState while((int error = glGetError()) != GL_NO_ERROR) log() - Log::Critical < "OpenGL error:" < error < ":" <
#endif
#ifndef DEBUG
#define assertf(x, y)
#endif

#endif //NGE_ASSERTIONS_HPP
