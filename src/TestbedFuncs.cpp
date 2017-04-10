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
				static const GLuint numVerts = GLuint(3);

				GLfloat vertData[numVerts][3][3] = {
					{ { -150.0f, -150.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
					{ { +000.0f, +150.0f, 0.0f }, { 0.5f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
					{ { +150.0f, -150.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
				};

				boost::container::vector<GLfloat> vertices;
				for (GLuint i = 0; i < numVerts; ++i)
				{
					for (GLuint j = 0; j < 3; ++j)
					{
						for (GLuint k = 0; k < 3; ++k)
						{
							vertices.push_back(vertData[i][j][k]);
						}
					}
				}

				triangleVAO			 = new VertexArray( );
				triangleVAO->BindVAO();
				GLsizei		  vboIdx = triangleVAO->CreateVertexBuffer( );
				VertexBuffer* vbo	= triangleVAO->GetVertexBuffer(vboIdx);
				if (vbo != nullptr)
				{

					vbo->BufferData(vertices, GL_STATIC_DRAW);
					vbo->BindVBO( );
					triangleVAO->SetupVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
													  RSYS_BUFR_OFST(0), false, false);
					triangleVAO->SetupVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
													  RSYS_BUFR_OFST(3 * sizeof(GLfloat)), false, false);
					triangleVAO->SetupVertexAttribute(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), RSYS_BUFR_OFST(6 * sizeof(GLfloat)), false, false);
					triangleVAO->SetCount(3);
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

		glm::mat4 proj =
			glm::ortho(fbSz.x / -2.0f, fbSz.x / 2.0f, fbSz.y / -2.0f, fbSz.y / 2.0f, -1.0f, 1.0f);

		glm::mat4 rot	  = glm::rotate(glm::mat4( ), glm::radians(-45.0f), glm::vec3(0, 0, 1));
		glm::mat4 modelMat = rot;

		glm::mat4 mvpMat = proj * modelMat;

		testShader->UniformMat4f("mvpMat", mvpMat);
		testShader->Uniform1i("tex", 1);
		testTex->BindTex(1);
		testSampler->BindSampler(1);

		triangleVAO->DrawVAO(GL_TRIANGLES);
		
		
		glBindVertexArray(0);
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
