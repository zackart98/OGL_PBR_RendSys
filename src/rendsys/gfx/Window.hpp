#ifndef RENDSYS_WINDOW_HPP_INC
#define RENDSYS_WINDOW_HPP_INC

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>



namespace rendsys
{



	// Window singleton class
	class Window
	{
	  public:
		// Get a singleton instance
		static Window& Inst( );


		// Set the internal GLFW window pointer
		void SetGLFWWindowPtr(GLFWwindow* winPtr);

		// Swap the OpenGL framebuffers for this window
		void SwapBuffers( );


		// Should the window close?
		bool ShouldQuit( );

		// Close the window early
		void CloseWindow( );

		// Get the framebuffer size
		glm::ivec2 FramebufferSize( );


		// Center the window on the given monitor
		void CenterOnMonitor(GLFWmonitor* monitor);

	  private:
		// Singleton instance
		static Window* instPtr;
		// Destoyed yet?
		static bool isDestroyed;


		// The GLFW window/context pointer
		GLFWwindow* windowPtr;

		// The size of the window in screen coordinates
		glm::ivec2 windowSize;

		// The size of the window in pixels
		glm::ivec2 framebufferSize;

		// The position of the window in screen coordinates
		glm::ivec2 windowPos;


		// Default ctor
		Window( );

		// Dtor
		~Window( );


		// Destroy the singleton instance at exit
		static void DestroyWindow( );


		Window(const Window& rhs);
		Window& operator=(const Window&);

		// GLFW framebuffer size callback
		static void FramebufferSizeCB_GLFW(GLFWwindow* win, int width, int height);

		// GLFW window size callback
		static void WindowSizeCB_GLFW(GLFWwindow* win, int width, int height);

		// GLFW window position callback
		static void WindowPosCB_GLFW(GLFWwindow* win, int x, int y);
	};
}

#endif
