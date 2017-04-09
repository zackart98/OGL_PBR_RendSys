#ifndef RENDSYS_TEXTURE_HPP_INC
#define RENDSYS_TEXTURE_HPP_INC

#include <rendsys/gfx/GL.hpp>

#include <boost/unordered_map.hpp>

#include <glm/glm.hpp>

#include <string>

namespace rendsys
{

	// Wrapper class around an OpenGL texture object
	class Texture
	{
	  public:
		// Default ctor
		Texture( );
		
		// Dtor
		virtual ~Texture( );


		// Load a texture from a file
		explicit Texture(const std::string& texPath);

		
		// Bind the texture to the specified texture unit
		void BindTex(GLuint texUnitNum);
		
		// Unbind this texture from the specified texture unit
		void UnbindTex(GLuint texUnitNum);
		
		
		// Get the maximum number of textures that can be passed into shaders
		static GLuint GetMaxCombinedTexUnits();
		
	  private:
		// The current state of an OpenGL multitexture unit
		struct TexBinding
		{
			Texture* boundTex;
			GLenum   texType;

			// Default ctor
			TexBinding( )
			{
				boundTex = nullptr;
				texType  = GLenum(0);
			}
		};

		// The states of all the multitexture units
		static boost::unordered_map<GLuint, TexBinding> allTexBindings;

		// The maximum number of textures that can be bound
		static GLuint maxCombinedTexUnits;
		
		// Internal OpenGL texture name
		GLuint textureID;

		// The OpenGL texture binding target to bind this texture to
		GLenum textureType;

		// Texture size
		glm::ivec2 texSize;

		// Initialize the map of texture bindings (allTexBindings)
		static void InitTexBindings( );
	};
}

#endif
