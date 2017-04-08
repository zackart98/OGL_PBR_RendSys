#ifndef RENDSYS_GL_HPP_INC
#define RENDSYS_GL_HPP_INC

#include <GL/glew.h>

#if defined(__linux__) || defined(__APPLE__)
#define RSYS_X11
#include <GL/glxew.h>
#endif

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || \
	defined(__BORLANDC__)
#define RSYS_WIN
#include <GL/wglew.h>
#endif

#define RSYS_BUFR_OFST(ofst) (reinterpret_cast<GLvoid*>(ofst))

#endif
