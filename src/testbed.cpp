#include <rendsys/gfx/GL.hpp>

#include <GLFW/glfw3.h>


#include <boost/cstdlib.hpp>

#include <rendsys/core/Initializer.hpp>
#include <rendsys/gfx/Window.hpp>

#include <TestbedFuncs.hpp>


int
main(int argc, char** argv)
{
	if (rendsys::Initializer::Init(glm::ivec2(1280, 720), "Rendering Testbed"))
	{
		tstbd::SetupTestbed( );

		double frameStartTime = glfwGetTime( );
		double deltaTime	  = 1.0 / 60.0;

		while (!rendsys::Window::Inst( ).ShouldQuit( ))
		{
			glfwPollEvents( );

			tstbd::UpdateTestbed(static_cast<float>(deltaTime));

			{
				glm::ivec2 fbSz = rendsys::Window::Inst( ).FramebufferSize( );
				glViewport(0, 0, fbSz.x, fbSz.y);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				tstbd::RenderTestbed( );
				rendsys::Window::Inst( ).SwapBuffers( );
			}

			{
				double now	 = glfwGetTime( );
				deltaTime	  = now - frameStartTime;
				frameStartTime = now;
			}
		}

		tstbd::CleanupTestbed( );
	}


	std::exit(boost::exit_success);
}
