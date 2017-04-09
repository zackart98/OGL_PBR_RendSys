#ifndef RENDSYS_SAMPLER_HPP_INC
#define RENDSYS_SAMPLER_HPP_INC

#include <rendsys/gfx/GL.hpp>

namespace rendsys
{

	// Wrapper class for an OpenGL sampler object
	class Sampler
	{
	  public:
		// Default ctor
		Sampler( );

		// Dtor
		virtual ~Sampler( );


		// Set the sampler parameter for 'GL_TEXTURE_WRAP_S' and
		// 'GL_TEXTURE_WRAP_T' to the same value
		void SetSamplerWrapST(GLenum wrapST);

		// Set the sampler parameter for 'GL_TEXTURE_WRAP_S',
		// 'GL_TEXTURE_WRAP_T' and 'GL_TEXTURE_WRAP_R' to the same value
		void SetSamplerWrapSTR(GLenum wrapSTR);

		// Set 'GL_TEXTURE_MIN_FILTER' and 'GL_TEXTURE_MAG_FILTER' at the same time,
		// to different values
		void SetSamplerFiltering(GLenum minFilter, GLenum magFilter);

		// Set 'GL_TEXTURE_MIN_FILTER'
		void SetSamplerMinFilter(GLenum minFilter);

		// Set 'GL_TEXTURE_MAG_FILTER'
		void SetSamplerMagFilter(GLenum magFilter);

		// Set 'GL_TEXTURE_MAX_ANISOTROPY_EXT' to allow for anisotropic filtering.
		void SetSamplerAnisotropy(GLfloat maxAnisotropy);

		// Set 'GL_TEXTURE_CUBE_MAP_SEAMLESS' to 'true' or 'false'
		void SetSamplerCubemapSeamless(bool doSeamless);

		
		// Bind the sampler to the specified texture unit
		void BindSampler(GLuint texUnitNum);
		
		// Unbind the sampler from the specified texture unit
		void UnbindSampler(GLuint texUnitNum);
		
	  private:
		// Internal OpenGL sampler object name
		GLuint samplerID;
	};
}

#endif
