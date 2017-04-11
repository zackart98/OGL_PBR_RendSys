#include <rendsys/core/InputHandler.hpp>
#include <rendsys/gfx/Window.hpp>

#include <boost/cstdlib.hpp>

#include <stdexcept>


namespace rendsys
{
	// Singleton instance
	InputHandler* InputHandler::instPtr = nullptr;
	// Destroyed yet?
	bool InputHandler::isDestroyed = false;


	// Default ctor
	InputHandler::InputHandler( )
	{
		isInit = false;
	}

	// Dtor
	InputHandler::~InputHandler( )
	{
	}

	// Initialize input: Inlcudes setting up GLFW callbacks
	void InputHandler::InitInput( )
	{
		if (!isInit)
		{
			glfwSetKeyCallback(Window::Inst( ).GetGLFWWindowPtr( ), &KeyCB_GLFW);

			isInit = true;
		}
	}

	// Register a new key callback
	void InputHandler::RegisterKeyCallback(int keycode, KeyCBFuncT cbFunc)
	{
		keyCallbacks[keycode].push_back(cbFunc);
	}

	// Simply calls 'glfwPollEvents'
	void InputHandler::PollEvents( )
	{
		glfwPollEvents();
	}


	// Get a singleton instance
	InputHandler& InputHandler::Inst( )
	{
		if (instPtr == nullptr)
		{
			if (!isDestroyed)
			{
				instPtr = new InputHandler( );
				std::atexit(&DestroyInputHandler);
			}
			else
			{
				throw std::runtime_error("Dead reference in singleton class 'InputHandler'!");
			}
		}
		return *(instPtr);
	}

	// Destroy the singleton instance at exit
	void InputHandler::DestroyInputHandler( )
	{
		if (instPtr != nullptr)
		{
			delete instPtr;
			instPtr		= nullptr;
			isDestroyed = true;
		}
	}


	// Implementation of the key callback
	void InputHandler::OnKey(int keycode, int action, int mods)
	{
		if (keyCallbacks.count(keycode))
		{
			boost::container::list<KeyCBFuncT>& callbacks = keyCallbacks[keycode];
			for (auto& cb : callbacks)
			{
				cb(action, mods);
			}
		}
	}

	// GLFW key callback
	void InputHandler::KeyCB_GLFW(GLFWwindow* win, int keycode, int scancode, int action, int mods)
	{
		if (win == Window::Inst( ).GetGLFWWindowPtr( ) && Inst( ).isInit)
		{
			Inst( ).OnKey(keycode, action, mods);
		}
	}
}
