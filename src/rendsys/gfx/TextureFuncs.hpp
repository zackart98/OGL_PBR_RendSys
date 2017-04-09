#ifndef RENDSYS_TEXTUREFUNCS_HPP_INC
#define RENDSYS_TEXTUREFUNCS_HPP_INC

#include <GL/glew.h>

#include <string>

namespace rendsys
{
	namespace detail
	{
		void InitFreeImage( );
		
		// Load an OpenGL texture from a file
		GLuint LoadGLTexture2D(const std::string& texturePath, GLsizei* texWOut, GLsizei* texHOut, bool genMipMaps = true);
	}
}

#endif
