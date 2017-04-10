#ifndef TESTBEDFUNCS_HPP
#define TESTBEDFUNCS_HPP

#include <rendsys/gfx/Shader.hpp>
#include <rendsys/gfx/Texture.hpp>
#include <rendsys/gfx/Sampler.hpp>
#include <rendsys/gfx/VertexArray.hpp>
#include <rendsys/gfx/VertexBuffer.hpp>

namespace tstbd
{
	extern rendsys::Shader* testShader;
	extern rendsys::Texture* testTex;
	extern rendsys::Sampler* testSampler;
	extern rendsys::VertexArray* squareVAO;
	
	extern GLsizei numSquaresX;
	extern GLsizei numSquaresY;
	extern GLsizei numSquares;
	
	void SetupTestbed();
	void RenderTestbed();
	void UpdateTestbed(float deltaTime);
	void CleanupTestbed();
}

#endif
