#include <TestbedFuncs.hpp>

#include <rendsys/gfx/Window.hpp>
#include <rendsys/gfx/Texture.hpp>


#include <glm/gtc/matrix_transform.hpp>


#include <iostream>

namespace tstbd
{
	using rendsys::Shader;
	using rendsys::Texture;
	using rendsys::Sampler;
	using rendsys::VertexArray;
	using rendsys::VertexBuffer;


	Shader*		 testShader	= nullptr;
	GLuint		 triangleVaoID = 0, triangleVboID = 0;
	Texture*	 testTex	 = nullptr;
	Sampler*	 testSampler = nullptr;
	VertexArray* triangleVAO = nullptr;

	void SetupTestbed( )
	{
		// ~~~~~~~~~~~~~~~~~~~~ Load shaders ~~~~~~~~~~~~~~~~~~~~
		{
			boost::unordered_map<GLenum, std::string> shaderPaths;
			{
				shaderPaths[GL_VERTEX_SHADER]   = "data/shaders/test/test.vert";
				shaderPaths[GL_FRAGMENT_SHADER] = "data/shaders/test/test.frag";
				testShader						= new Shader(shaderPaths);
				shaderPaths.clear( );
			}
		}

		// ~~~~~~~~~~~~~~~~~~~~~ VAOs ~~~~~~~~~~~~~~~~~~~~~
		{

			// Debug triangle VAO
			{

				boost::container::vector<glm::vec3> positions = { glm::vec3(125, 125, 00),
																  glm::vec3(125, 00, 00),
																  glm::vec3(00, 00, 00),
																  glm::vec3(00, 125, 00) };

				boost::container::vector<glm::vec2> texCoords = { glm::vec2(1.0f, 0.0f),
																  glm::vec2(1.0f, 1.0f),
																  glm::vec2(0.0f, 1.0f),
																  glm::vec2(0.0f, 0.0f) };

				triangleVAO = new VertexArray( );
				triangleVAO->BindVAO( );

				GLsizei		  posVboIdx = triangleVAO->CreateVertexBuffer( );
				VertexBuffer* posVbo	= triangleVAO->GetVertexBuffer(posVboIdx);

				GLsizei		  tcVboIdx = triangleVAO->CreateVertexBuffer( );
				VertexBuffer* tcVbo	= triangleVAO->GetVertexBuffer(tcVboIdx);

				boost::container::vector<glm::mat4> modelMats;
				for (GLint r = 0; r < 5; ++r)
				{
					for (GLint c = 0; c < 5; ++c)
					{
						glm::mat4 model = glm::translate(glm::mat4( ), glm::vec3(c, r, 0) * 130.0f + glm::vec3(5.0f, 5.0f, 0.0f));
						modelMats.push_back(model);
					}
				}

				GLsizei		  mmatVboIdx = triangleVAO->CreateVertexBuffer( );
				VertexBuffer* mmatVbo	= triangleVAO->GetVertexBuffer(mmatVboIdx);

				if (posVbo != nullptr && tcVbo != nullptr)
				{

					posVbo->BufferData(positions, GL_STATIC_DRAW);
					posVbo->BindVBO( );
					triangleVAO->SetupVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
													  RSYS_BUFR_OFST(0), false, false);

					tcVbo->BufferData(texCoords, GL_STATIC_DRAW);
					tcVbo->BindVBO( );
					triangleVAO->SetupVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
													  RSYS_BUFR_OFST(0), false, false);

					mmatVbo->BufferData(modelMats, GL_STATIC_DRAW);
					mmatVbo->BindVBO( );
					triangleVAO->SetupVertexAttribute(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
													  RSYS_BUFR_OFST(0), true, true);

					triangleVAO->SetCount(4);
				}
			}
		}

		testTex = new Texture("data/tex/wall.jpg");

		{
			testSampler = new Sampler( );
			testSampler->SetSamplerFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
			testSampler->SetSamplerWrapST(GL_REPEAT);
			testSampler->SetSamplerAnisotropy(16.0f);
		}
	}

	void RenderTestbed( )
	{
		testShader->BindShader( );

		boost::container::vector<glm::vec3> colors = { glm::vec3(1, 0, 0), glm::vec3(1, 1, 0),
													   glm::vec3(0, 0, 1) };

		//testShader->Uniform3fv("colors", colors);

		glm::vec2 fbSz(rendsys::Window::Inst( ).FramebufferSize( ));

		glm::mat4 proj = glm::ortho(0.0f, fbSz.x, fbSz.y, 0.0f, -1.0f, 1.0f);

		glm::mat4 rot	  = glm::rotate(glm::mat4( ), glm::radians(-45.0f), glm::vec3(0, 0, 1));
		glm::mat4 modelMat = rot;

		glm::mat4 mvpMat = proj;

		testShader->UniformMat4f("projMat", proj);
		testShader->Uniform1i("tex", 1);
		testTex->BindTex(1);
		testSampler->BindSampler(1);

		triangleVAO->DrawVAOInst(GL_TRIANGLE_FAN, 25);

		testShader->UnbindShader( );
		testTex->UnbindTex(1);
	}

	void UpdateTestbed(float deltaTime)
	{
	}

	void CleanupTestbed( )
	{
		if (testShader != nullptr)
		{
			delete testShader;
			testShader = nullptr;
		}

		if (triangleVaoID != 0)
		{
			glDeleteVertexArrays(1, &triangleVaoID);
			glDeleteBuffers(1, &triangleVboID);
			triangleVaoID = triangleVboID = 0;
		}

		if (testTex != nullptr)
		{
			delete testTex;
			testTex = nullptr;
		}

		if (testSampler != nullptr)
		{
			delete testSampler;
			testSampler = nullptr;
		}
	}
}
