#include <rendsys/gfx/GL.hpp>

#include <GLFW/glfw3.h>

#include <rendsys/Config.hpp>
#include <rendsys/core/Initializer.hpp>
#include <rendsys/gfx/TextureFuncs.hpp>
#include <rendsys/gfx/Window.hpp>
#include <rendsys/core/InputHandler.hpp>


#include <iostream>

namespace rendsys
{
	bool Initializer::Init(const glm::ivec2& windowSize, const char* windowTitle)
	{
		bool success = true;
		success		 = success && InitGLFW(windowSize, windowTitle);

		// Initialize OpenGL
		InitGL( );

		// Initialize FreeImage library
		detail::InitFreeImage( );

		InputHandler::Inst().InitInput();
		
		return success;
	}

	bool Initializer::InitGLFW(const glm::ivec2& windowSize, const char* windowTitle)
	{
		bool success = true;

		glfwSetErrorCallback(&ErrorCB_GLFW);

		// Initialize GLFW, set success flag to 'false' on failure
		if (!glfwInit( ))
		{
			success = false;
		}

		int majorVersion = 4;
		int minorVersion;

		// Find highest appropriate available OpenGL version
		{
			if (GL_VERSION_4_5)
			{
				minorVersion = 5;
			}
			else if (GL_VERSION_4_4)
			{
				minorVersion = 4;
			}
			else if (GL_VERSION_4_3)
			{
				minorVersion = 3;
			}
#ifndef RSYS_NO_OLD_HARDWARE
			else if (GL_VERSION_3_3)
			{
				majorVersion = 3;
				minorVersion = 3;
			}
#endif
			else
			{
				success = false;
			}
		}

		if (success)
		{
			// Set window hints
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_DEPTH_BITS, 24);
			glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

			// Create window
			GLFWwindow* window =
				glfwCreateWindow(windowSize.x, windowSize.y, windowTitle, nullptr, nullptr);

			// Window was not created successfully
			if (window == nullptr)
			{
				success = false;
			}
			else
			{
				// Set window singleton internal GLFW window/context pointer
				Window::Inst( ).SetGLFWWindowPtr(window);

				Window::Inst( ).CenterOnMonitor(glfwGetPrimaryMonitor( ));

				// Activate context
				glfwMakeContextCurrent(window);

				// Initialize GLEW
				glewInit( );

				// Swap interval
				{
#if defined(RSYS_X11) // X11 window system
					if (glxewIsSupported("GLX_EXT_swap_control_tear"))
					{
						glfwSwapInterval(-1);
						std::cout << "Adaptive Vsync enabled" << std::endl;
					}
#elif defined(RSYS_WIN) // Windows
					if (wglewIsSupported("WGL_EXT_swap_control_tear"))
					{
						glfwSwapInterval(-1);
						std::cout << "Adaptive Vsync enabled" << std::endl;
					}
#else

#error Unsupported platform!

#endif
					else // Adaptive V-sync not supported, enable regular V-sync
					{
						glfwSwapInterval(1);
						std::cout << "Vsync enabled" << std::endl;
					}
				}
			}
		}

		return success;
	}

	// Initialize OpenGL
	void Initializer::InitGL( )
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glm::ivec2 fbSize = Window::Inst( ).FramebufferSize( );
		glViewport(0, 0, fbSize.x, fbSize.y);

		glEnable(GL_DEPTH_TEST);
	}

	// GLFW error callback
	void Initializer::ErrorCB_GLFW(int code, const char* description)
	{
		std::cerr << "ERROR!::GLFW(" << code << ") -- " << description << std::endl << std::endl;
	}
}
