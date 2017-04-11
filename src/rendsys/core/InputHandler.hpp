#ifndef RENDSYS_INPUTHANDLER_HPP_INC
#define RENDSYS_INPUTHANDLER_HPP_INC

#include <GLFW/glfw3.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <boost/unordered_map.hpp>
#include <boost/container/list.hpp>

namespace rendsys
{

	typedef boost::function<void(int, int)> KeyCBFuncT;

	// Singleton class that handles user input
	class InputHandler
	{
	  public:
		// Get a singleton instance
		static InputHandler& Inst( );


		// Initialize input: Inlcudes setting up GLFW callbacks
		void InitInput( );
		
		// Register a new key callback
		void RegisterKeyCallback(int keycode, KeyCBFuncT cbFunc);

		// Simply calls 'glfwPollEvents'
		void PollEvents();
		
	  private:
		// Singleton instance
		static InputHandler* instPtr;

		// Destroyed yet?
		static bool isDestroyed;


		// Initialized yet?
		bool isInit;

		// All the registered key callback functions to forward GLFW key callacks to
		boost::unordered_map<int, boost::container::list<KeyCBFuncT>> keyCallbacks;
		
		
		// Implementation of the key callback
		void OnKey(int keycode, int action, int mods);
		

		// Default ctor
		InputHandler( );

		// Dtor
		~InputHandler( );

		InputHandler(const InputHandler& rhs);
		InputHandler& operator=(const InputHandler&);

		// Destroy the singleton instance at exit
		static void DestroyInputHandler( );



		// GLFW key callback
		static void KeyCB_GLFW(GLFWwindow* win, int keycode, int scancode, int action, int mods);
	};
}

#endif
