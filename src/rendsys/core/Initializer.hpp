#ifndef RENDSYS_INITIALIZER_HPP_INC
#define RENDSYS_INITIALIZER_HPP_INC



#include <glm/glm.hpp>

namespace rendsys
{



	// Static class to initialize the rendering system
	class Initializer
	{
	  public:
		// Initialize the rendering system
		static bool Init(const glm::ivec2& windowSize, const char* windowTitle);

	  private:
		// Initialize GLFW
		static bool InitGLFW(const glm::ivec2& windowSize, const char* windowTitle);

		// Initialize OpenGL
		static void InitGL( );

		// GLFW error callback
		static void ErrorCB_GLFW(int code, const char* description);
	};
}

#endif
