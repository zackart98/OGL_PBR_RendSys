#include <TestbedFuncs.hpp>

#include <rendsys/gfx/Window.hpp>
#include <rendsys/gfx/Texture.hpp>


#include <glm/gtc/matrix_transform.hpp>

#include <boost/range/algorithm/reverse.hpp>

#include <iostream>

namespace tstbd
{
	using rendsys::Shader;
	using rendsys::Texture;
	using rendsys::Sampler;
	using rendsys::VertexArray;
	using rendsys::VertexBuffer;


	Shader*		 testShader  = nullptr;
	Texture*	 testTex	 = nullptr;
	Sampler*	 testSampler = nullptr;
	VertexArray* squareVAO   = nullptr;

	GLsizei numSquaresX = 8;
	GLsizei numSquaresY = 8;
	GLsizei numSquares  = numSquaresX * numSquaresY;

	void SetupTestbed( )
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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

			// Debug square VAO
			{

				GLfloat squareSize	= 80.0f;
				GLfloat squarePadding = 8.0f;

				boost::container::vector<glm::vec3> positions = {
					glm::vec3(squareSize, squareSize, 00), glm::vec3(squareSize, 00, 00),
					glm::vec3(00, 00, 00), glm::vec3(00, squareSize, 00)
				};

				boost::container::vector<glm::vec2> texCoords = { glm::vec2(1.0f, 0.0f),
																  glm::vec2(1.0f, 1.0f),
																  glm::vec2(0.0f, 1.0f),
																  glm::vec2(0.0f, 0.0f) };

				squareVAO = new VertexArray( );
				squareVAO->BindVAO( );

				GLsizei		  posVboIdx = squareVAO->CreateVertexBuffer( );
				VertexBuffer* posVbo	= squareVAO->GetVertexBuffer(posVboIdx);

				GLsizei		  tcVboIdx = squareVAO->CreateVertexBuffer( );
				VertexBuffer* tcVbo	= squareVAO->GetVertexBuffer(tcVboIdx);

				boost::container::vector<glm::mat4> modelMats;
				GLint instIdx = numSquares;
				for (GLint r = 0; r < numSquaresY; ++r)
				{
					for (GLint c = 0; c < numSquaresX; ++c)
					{
						glm::mat4 trans = glm::translate(
							glm::mat4( ), glm::vec3(c, r, 0) * (squareSize + squarePadding) +
											  glm::vec3(squarePadding, squarePadding, 0.0f));

						glm::vec3 sclAmt = glm::vec3(instIdx, instIdx, 0) / static_cast<GLfloat>(numSquares);
						glm::mat4 scal = glm::scale(glm::mat4( ), sclAmt);

						glm::mat4 model = trans * scal;

						modelMats.push_back(model);
						instIdx--;
					}
				}


				GLsizei		  mmatVboIdx = squareVAO->CreateVertexBuffer( );
				VertexBuffer* mmatVbo	= squareVAO->GetVertexBuffer(mmatVboIdx);

				if (posVbo != nullptr && tcVbo != nullptr)
				{

					posVbo->BufferData(positions, GL_STATIC_DRAW);
					posVbo->BindVBO( );
					squareVAO->SetupVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
													RSYS_BUFR_OFST(0), false, false);

					tcVbo->BufferData(texCoords, GL_STATIC_DRAW);
					tcVbo->BindVBO( );
					squareVAO->SetupVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
													RSYS_BUFR_OFST(0), false, false);

					mmatVbo->BufferData(modelMats, GL_STATIC_DRAW);
					mmatVbo->BindVBO( );
					squareVAO->SetupVertexAttribute(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
													RSYS_BUFR_OFST(0), true, true);

					squareVAO->SetCount(4);
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

		glm::mat4 projMat = glm::ortho(0.0f, fbSz.x, fbSz.y, 0.0f, -1.0f, 1.0f);

		glm::mat4 rot	  = glm::rotate(glm::mat4( ), glm::radians(-45.0f), glm::vec3(0, 0, 1));
		glm::mat4 modelMat = rot;
		//glm::mat4 mvpMat   = projMat * modelMat;

		testShader->UniformMat4f("projMat", projMat);
		testShader->Uniform1i("tex", 1);
		testTex->BindTex(1);
		testSampler->BindSampler(1);

		squareVAO->DrawVAOInst(GL_TRIANGLE_FAN, numSquares);

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

		if (squareVAO != nullptr)
		{
			delete squareVAO;
			squareVAO = nullptr;
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
