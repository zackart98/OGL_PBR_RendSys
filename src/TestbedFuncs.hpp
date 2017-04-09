#ifndef TESTBEDFUNCS_HPP
#define TESTBEDFUNCS_HPP

#include <rendsys/gfx/Shader.hpp>
#include <rendsys/gfx/Texture.hpp>
#include <rendsys/gfx/Sampler.hpp>

namespace tstbd
{
	extern rendsys::Shader* testShader;
	extern GLuint triangleVaoID, triangleVboID;
	extern rendsys::Texture* testTex;
	extern rendsys::Sampler* testSampler;
	
	void SetupTestbed();
	void RenderTestbed();
	void UpdateTestbed(float deltaTime);
	void CleanupTestbed();
}

#endif
