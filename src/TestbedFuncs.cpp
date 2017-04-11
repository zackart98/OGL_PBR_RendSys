#include <TestbedFuncs.hpp>

#include <rendsys/gfx/Window.hpp>
#include <rendsys/gfx/Texture.hpp>
#include <rendsys/core/InputHandler.hpp>

#include <rendsys/math/Transform.hpp>

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
	using rendsys::StaticModel;
	using rendsys::Window;
	using rendsys::InputHandler;
	using rendsys::Transform;

	Shader*		 testShader	= nullptr;
	Texture*	 testTex	   = nullptr;
	Sampler*	 testSampler   = nullptr;
	VertexArray* squareVAO	 = nullptr;
	StaticModel* nanosuitModel = nullptr;
	float yaw;

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
				GLint								instIdx = numSquares;
				for (GLint r = 0; r < numSquaresY; ++r)
				{
					for (GLint c = 0; c < numSquaresX; ++c)
					{
						glm::mat4 trans = glm::translate(
							glm::mat4( ), glm::vec3(c, r, 0) * (squareSize + squarePadding) +
											  glm::vec3(squarePadding, squarePadding, 0.0f));

						glm::vec3 sclAmt =
							glm::vec3(instIdx, instIdx, 0) / static_cast<GLfloat>(numSquares);
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

		{
			nanosuitModel = new StaticModel("data/models/nanosuit/nanosuit.obj");
		}
		
		{
			InputHandler::Inst().RegisterKeyCallback(GLFW_KEY_ESCAPE, OnEscapeKey);
		}
	}
	
	

	void RenderTestbed( )
	{
		Transform viewTransform;
		viewTransform.translate = glm::vec3(0, 0, 3.0);
		viewTransform.SetYaw(yaw);
		viewTransform.SetPitch(0);
		viewTransform.SetRoll(0);
		
		glm::vec2 fbSz(Window::Inst( ).FramebufferSize( ));
		glm::mat4 projMat = glm::perspective(glm::radians(45.0f), (fbSz.x / fbSz.y), 0.1f, 100.0f);
		glm::mat4 viewMat = viewTransform.GetLookAtMatrix();
		glm::mat4 modelMat = glm::scale(glm::mat4( ), glm::vec3(0.6, 0.6, 0.6));

		boost::container::vector<glm::mat4> modelMats = { modelMat };
		nanosuitModel->SetModelMats(modelMats);

		testShader->BindShader( );
		testShader->UniformMat4f("projMat", projMat);
		testShader->UniformMat4f("viewMat", viewMat);
		testShader->Uniform3f("color", glm::vec3(1.0f, 1.0f, 0.0f));
		
		nanosuitModel->DrawModel();
		
		testShader->UnbindShader( );
	}

	void UpdateTestbed(float deltaTime)
	{
		yaw += deltaTime * 15.0f;
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
	
	void OnEscapeKey(int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			GLFWwindow* win = Window::Inst().GetGLFWWindowPtr();
			glfwSetWindowShouldClose(win, GLFW_TRUE);
		}
	}
}
