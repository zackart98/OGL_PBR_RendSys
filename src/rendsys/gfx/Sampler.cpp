#include <rendsys/gfx/Sampler.hpp>
#include <rendsys/gfx/Texture.hpp>


namespace rendsys
{
	// Default ctor
	Sampler::Sampler( )
	{
		samplerID = 0;
		glGenSamplers(1, &samplerID);
	}

	Sampler::~Sampler( )
	{
		if (samplerID != 0)
		{
			glDeleteSamplers(1, &samplerID);
			samplerID = 0;
		}
	}

	// Set the sampler parameter for 'GL_TEXTURE_WRAP_S' and
	// 'GL_TEXTURE_WRAP_T' to the same value
	void Sampler::SetSamplerWrapST(GLenum wrapST)
	{
		if (glIsSampler(samplerID))
		{
			glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GLint(wrapST));
			glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GLint(wrapST));
		}
	}

	// Set the sampler parameter for 'GL_TEXTURE_WRAP_S',
	// 'GL_TEXTURE_WRAP_T' and 'GL_TEXTURE_WRAP_R' to the same value
	void Sampler::SetSamplerWrapSTR(GLenum wrapSTR)
	{
		if (glIsSampler(samplerID))
		{
			glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GLint(wrapSTR));
			glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GLint(wrapSTR));
			glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_R, GLint(wrapSTR));
		}
	}

	// Set 'GL_TEXTURE_MIN_FILTER' and 'GL_TEXTURE_MAG_FILTER' at the same time,
	// to different values
	void Sampler::SetSamplerFiltering(GLenum minFilter, GLenum magFilter)
	{
		if (glIsSampler(samplerID))
		{
			glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GLint(minFilter));
			glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GLint(magFilter));
		}
	}

	// Set 'GL_TEXTURE_MIN_FILTER'
	void Sampler::SetSamplerMinFilter(GLenum minFilter)
	{
		if (glIsSampler(samplerID))
		{
			glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GLint(minFilter));
		}
	}

	// Set 'GL_TEXTURE_MAG_FILTER'
	void Sampler::SetSamplerMagFilter(GLenum magFilter)
	{
		if (glIsSampler(samplerID))
		{
			glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GLint(magFilter));
		}
	}

	// Set 'GL_TEXTURE_MAX_ANISOTROPY_EXT' to allow for anisotropic filtering.
	void Sampler::SetSamplerAnisotropy(GLfloat maxAnisotropy)
	{
#if defined(GL_EXT_texture_filter_anisotropic) && (GL_EXT_texture_filter_anisotropic) == 1
		{
			if (glIsSampler(samplerID) && maxAnisotropy >= 1.0f)
			{
				glSamplerParameterf(samplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
		}
#endif
	}

	// Set 'GL_TEXTURE_CUBE_MAP_SEAMLESS' to 'true' or 'false'
	void Sampler::SetSamplerCubemapSeamless(bool doSeamless)
	{
		if (glIsSampler(samplerID))
		{
			glSamplerParameteri(samplerID, GL_TEXTURE_CUBE_MAP_SEAMLESS,
								(doSeamless) ? GLint(GL_TRUE) : GLint(GL_FALSE));
		}
	}


	// Bind the sampler to the specified texture unit
	void Sampler::BindSampler(GLuint texUnitNum)
	{
		GLuint maxCombined = Texture::GetMaxCombinedTexUnits( );
		if (texUnitNum < maxCombined)
		{
			GLint lastTexUnit;
			glGetIntegerv(GL_ACTIVE_TEXTURE, &lastTexUnit);

			glActiveTexture(GL_TEXTURE0 + texUnitNum);
			GLint lastSampler;
			glGetIntegerv(GL_SAMPLER_BINDING, &lastSampler);
			glActiveTexture(lastTexUnit);
			if (GLuint(lastSampler) != samplerID)
			{
				glBindSampler(texUnitNum, samplerID);
			}
		}
	}

	// Unbind the sampler from the specified texture unit
	void Sampler::UnbindSampler(GLuint texUnitNum)
	{
		GLuint maxCombined = Texture::GetMaxCombinedTexUnits( );
		if (texUnitNum < maxCombined)
		{
			GLint lastTexUnit;
			glGetIntegerv(GL_ACTIVE_TEXTURE, &lastTexUnit);

			glActiveTexture(GL_TEXTURE0 + texUnitNum);
			GLint lastSampler;
			glGetIntegerv(GL_SAMPLER_BINDING, &lastSampler);
			glActiveTexture(lastTexUnit);
			if (GLuint(lastSampler) == samplerID)
			{
				glBindSampler(texUnitNum, 0);
			}
		}
	}
}
