#ifndef RENDSYS_SHADER_HPP_INC
#define RENDSYS_SHADER_HPP_INC

#include <rendsys/gfx/GL.hpp>

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/unordered_map.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include <boost/container/vector.hpp>

namespace rendsys
{

	// Wrapper class for an OpenGL shader object
	class Shader final
	{
	  public:
		// Default ctor
		Shader( );
		
		// Dtor
		~Shader();

		// Construct from paths
		explicit Shader(const boost::unordered_map<GLenum, std::string>& shaderPaths);

		// Load the shader program from a a hash map that maps shader types to source file paths
		bool LoadShader(const boost::unordered_map<GLenum, std::string>& shaderPaths);


		// Bind this shader for use
		void BindShader();
		
		// If this shader is bound, call 'glUseProgram(0)'
		void UnbindShader();
		
		
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Uniform Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		void Uniform1i(const std::string& uniName, GLint v0);
		
		void Uniform1f(const std::string& uniName, GLfloat v0);
		void Uniform2f(const std::string& uniName, GLfloat v0, GLfloat v1);
		void Uniform3f(const std::string& uniName, GLfloat v0, GLfloat v1, GLfloat v2);
		void Uniform4f(const std::string& uniName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		
		void Uniform2f(const std::string& uniName, const glm::vec2& v);
		void Uniform2fv(const std::string& uniName, const boost::container::vector<glm::vec2>& v);
		
		void Uniform3f(const std::string& uniName, const glm::vec3& v);
		void Uniform3fv(const std::string& uniName, const boost::container::vector<glm::vec3>& v);
		
		void Uniform4f(const std::string& uniName, const glm::vec4& v);
		void Uniform4fv(const std::string& uniName, const boost::container::vector<glm::vec4>& v);
		
		void UniformMat4f(const std::string& uniName, const glm::mat4& m);
		void UniformMat4fv(const std::string& uniName, const boost::container::vector<glm::mat4>& m);
		
	  private:
		
		static boost::unordered_map<GLenum, std::string> allShaderTypes;
		static GLuint currentProgramID;
		
		// Uniform metadata
		struct UniformData
		{
			GLint  location = -1;
			GLenum type		= 0;
		};

		// The OpenGL name for the internal shader program
		GLuint programID;

		// Uniform metadata
		boost::unordered_map<std::string, UniformData> uniforms;
	};
}

#endif
