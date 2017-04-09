#include <rendsys/gfx/Texture.hpp>

#include <rendsys/gfx/TextureFuncs.hpp>

#include <iostream>

namespace rendsys
{
	// The states of all the multitexture units
	boost::unordered_map<GLuint, Texture::TexBinding> Texture::allTexBindings =
		boost::unordered_map<GLuint, Texture::TexBinding>( );

	// Default ctor
	Texture::Texture( )
	{
		InitTexBindings( );
		textureID   = 0;
		textureType = 0;
		texSize		= glm::ivec2(-1, -1);
	}

	// Load a texture from a file
	Texture::Texture(const std::string& texPath)
	{
		InitTexBindings( );
		textureID   = detail::LoadGLTexture2D(texPath, &(texSize.x), &(texSize.y), true);
		textureType = (textureType > 0) ? (GL_TEXTURE_2D) : (GL_FALSE);
	}

	// Bind  the texture to the specified texture unit
	void Texture::BindTex(GLuint texUnitNum)
	{
		if (allTexBindings.count(texUnitNum) > 0 && allTexBindings[texUnitNum].boundTex != this)
		{
			if (glIsTexture(textureID))
			{
				GLint lastActiveTex;
				glGetIntegerv(GL_ACTIVE_TEXTURE, &lastActiveTex);

				glActiveTexture(GL_TEXTURE0 + texUnitNum);
				glBindTexture(textureType, textureID);
				glActiveTexture(lastActiveTex);

				allTexBindings[texUnitNum].boundTex = this;
				allTexBindings[texUnitNum].texType  = textureType;
			}
		}
	}

	// Unbind this texture from the specified texture unit
	void Texture::UnbindTex(GLuint texUnitNum)
	{
		if (allTexBindings.count(texUnitNum) > 0 && allTexBindings[texUnitNum].boundTex == this)
		{
			GLint lastActiveTex;
			glGetIntegerv(GL_ACTIVE_TEXTURE, &lastActiveTex);

			glActiveTexture(GL_TEXTURE0 + texUnitNum);
			glBindTexture(textureType, 0);
			glActiveTexture(lastActiveTex);

			allTexBindings[texUnitNum].boundTex = nullptr;
			allTexBindings[texUnitNum].texType  = GL_FALSE;
		}
	}

	// Initialize the map of texture bindings (allTexBindings)
	void Texture::InitTexBindings( )
	{
		if (allTexBindings.empty( ))
		{
			GLint maxCombined = 0;
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombined);

			//std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << maxCombined << std::endl;

			if (maxCombined > 0)
			{
				for (GLuint i = 0; i < GLuint(maxCombined); ++i)
				{
					TexBinding binding;
					binding.boundTex  = nullptr;
					binding.texType   = GL_FALSE;
					allTexBindings[i] = binding;
				}
			}
		}
	}
};
