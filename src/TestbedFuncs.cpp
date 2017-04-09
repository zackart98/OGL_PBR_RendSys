#include <TestbedFuncs.hpp>
#include <rendsys/gfx/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace tstbd
{
	using rendsys::Shader;

	Shader* testShader	= nullptr;
	GLuint  triangleVaoID = 0, triangleVboID = 0;

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
				GLfloat vertices[][3][3] = {
					{ { -150.0f, -150.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
					{ { +000.0f, +150.0f, 0.0f }, { 0.5f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
					{ { +150.0f, -150.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
				};

				glGenVertexArrays(1, &triangleVaoID);
				glGenBuffers(1, &triangleVboID);

				glBindVertexArray(triangleVaoID);
				{
					glBindBuffer(GL_ARRAY_BUFFER, triangleVboID);
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(GLfloat)),
										  RSYS_BUFR_OFST(0));

					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(GLfloat)),
										  RSYS_BUFR_OFST(3 * sizeof(GLfloat)));

					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(GLfloat)),
										  RSYS_BUFR_OFST(6 * sizeof(GLfloat)));
				}
				glBindVertexArray(0);
			}
		}
	}

	void RenderTestbed( )
	{
		testShader->BindShader( );

		boost::container::vector<glm::vec3> colors = { glm::vec3(1, 0, 0), glm::vec3(1, 1, 0),
													   glm::vec3(0, 0, 1) };
		
		testShader->Uniform3fv("colors", colors);
		
		glm::vec2 fbSz(rendsys::Window::Inst().FramebufferSize());
		
		boost::container::vector<glm::mat4> mvpMats = {
			glm::mat4(1.0f),
			glm::mat4(1.0f),
			glm::mat4(1.0f)
		};
		
		testShader->UniformMat4f("mvpMat", glm::ortho(fbSz.x / -2.0f, fbSz.x / 2.0f, fbSz.y / -2.0f, fbSz.y / 2.0f, -1.0f, 1.0f));
		
		glBindVertexArray(triangleVaoID);
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);
		testShader->UnbindShader( );
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
	}
}
