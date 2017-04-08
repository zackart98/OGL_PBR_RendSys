#ifndef TESTBEDFUNCS_HPP
#define TESTBEDFUNCS_HPP

#include <rendsys/gfx/Shader.hpp>

namespace tstbd
{
	extern rendsys::Shader* testShader;
	extern GLuint triangleVaoID, triangleVboID;
	
	void SetupTestbed();
	void RenderTestbed();
	void UpdateTestbed(float deltaTime);
	void CleanupTestbed();
}

#endif
