#include <rendsys/gfx/GL.hpp>

#include <rendsys/gfx/Window.hpp>

#include <boost/cstdlib.hpp>

#include <stdexcept>

namespace rendsys
{
	// Singleton instance
	Window* Window::instPtr = nullptr;
	// Destoyed yet?
	bool Window::isDestroyed = false;

	// Default ctor
	Window::Window( )
	{
		windowPtr		= nullptr;
		windowSize		= glm::ivec2(-1, -1);
		framebufferSize = glm::ivec2(-1, -1);
		windowPos		= glm::ivec2(-1, -1);
	}

	// Dtor
	Window::~Window( )
	{
		glfwDestroyWindow(windowPtr);
		glfwTerminate( );

		windowSize		= glm::ivec2(-1, -1);
		framebufferSize = glm::ivec2(-1, -1);
	}

	// Get a singleton instance
	Window& Window::Inst( )
	{
		if (instPtr == nullptr)
		{
			if (!isDestroyed)
			{
				instPtr = new Window( );
				std::atexit(&DestroyWindow);
			}
			else
			{
				throw std::runtime_error("Dead reference in singleton class 'Window'!");
			}
		}
		return *(instPtr);
	}

	// Set the internal GLFW window pointer
	void Window::SetGLFWWindowPtr(GLFWwindow* winPtr)
	{
		if (winPtr != windowPtr && winPtr != nullptr)
		{
			windowPtr = winPtr;

			glfwGetWindowSize(windowPtr, &windowSize.x, &windowSize.y);
			glfwGetFramebufferSize(windowPtr, &framebufferSize.x, &framebufferSize.y);
			glfwGetWindowPos(windowPtr, &windowPos.x, &windowPos.y);

			glfwSetWindowSizeCallback(windowPtr, &WindowSizeCB_GLFW);
			glfwSetFramebufferSizeCallback(windowPtr, &FramebufferSizeCB_GLFW);
			glfwSetWindowPosCallback(windowPtr, &WindowPosCB_GLFW);
		}
	}
	// Get the internal GLFW window
	GLFWwindow* Window::GetGLFWWindowPtr( )
	{
		return windowPtr;
	}

	// Swap the OpenGL framebuffers for this window
	void Window::SwapBuffers( )
	{
		if (windowPtr != nullptr)
		{
			glfwSwapBuffers(windowPtr);
		}
	}

	// Should the window close?
	bool Window::ShouldQuit( )
	{
		if (windowPtr != nullptr)
		{
			return glfwWindowShouldClose(windowPtr);
		}
		return true;
	}

	// Close the window early
	void Window::CloseWindow( )
	{
		glfwSetWindowShouldClose(windowPtr, true);
	}


	// Get the framebuffer size
	glm::ivec2 Window::FramebufferSize( )
	{
		return framebufferSize;
	}



	// Center the window on the given monitor
	void Window::CenterOnMonitor(GLFWmonitor* monitor)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glm::ivec2		   monSiz(mode->width, mode->height);
		glm::ivec2		   monPos;
		glfwGetMonitorPos(monitor, &monPos.x, &monPos.y);

		windowPos = ((monSiz - windowSize) / 2) + monPos;
		glfwSetWindowPos(windowPtr, windowPos.x, windowPos.y);
	}


	// Destroy the singleton instance at exit
	void Window::DestroyWindow( )
	{
		if (instPtr != nullptr)
		{
			delete instPtr;
			instPtr		= nullptr;
			isDestroyed = true;
		}
	}

	// GLFW framebuffer size callback
	void Window::FramebufferSizeCB_GLFW(GLFWwindow* win, int width, int height)
	{
		if (win == Inst( ).windowPtr)
		{
			Inst( ).framebufferSize = glm::ivec2(width, height);
			glViewport(0, 0, width, height);
		}
	}

	// GLFW window size callback
	void Window::WindowSizeCB_GLFW(GLFWwindow* win, int width, int height)
	{
		if (win == Inst( ).windowPtr)
		{
			Inst( ).windowSize = glm::ivec2(width, height);
		}
	}

	// GLFW window position callback
	void Window::WindowPosCB_GLFW(GLFWwindow* win, int x, int y)
	{
		if (win == Inst( ).windowPtr)
		{
			Inst( ).windowPos = glm::ivec2(x, y);
		}
	}
}
