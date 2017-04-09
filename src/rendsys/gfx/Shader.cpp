#include <rendsys/gfx/Shader.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <streambuf>

namespace rendsys
{
	boost::unordered_map<GLenum, std::string> Shader::allShaderTypes = {
		{ GL_VERTEX_SHADER, "vertex" },
		{ GL_FRAGMENT_SHADER, "fragment" },
		{ GL_GEOMETRY_SHADER, "geometry" },
		{ GL_TESS_CONTROL_SHADER, "tessellation control" },
		{ GL_TESS_EVALUATION_SHADER, "tessellation evaluation" }
	};

	GLuint Shader::currentProgramID = 0;

	// Default ctor
	Shader::Shader( )
	{
		programID = 0;
		uniforms  = boost::unordered_map<std::string, UniformData>( );
	}

	// Construct from paths
	Shader::Shader(const boost::unordered_map<GLenum, std::string>& shaderPaths)
	{
		LoadShader(shaderPaths);
	}

	// Load the shader program from a a hash map that maps shader types to source file paths
	bool Shader::LoadShader(const boost::unordered_map<GLenum, std::string>& shaderPaths)
	{
		// Input file stream
		typedef boost::iostreams::stream<boost::iostreams::file_source> ifstream;

		bool success = true;

		// The shader objects that have been loaded
		boost::container::vector<GLuint> shaderIDs;

		bool loadedVert = false;
		bool loadedFrag = false;

		boost::container::vector<GLchar> infoLog;

		for (auto& shdrPth : shaderPaths)
		{
			GLenum		shaderType = shdrPth.first;
			std::string shdrPath   = shdrPth.second;

			if (allShaderTypes.count(shaderType))
			{
				if (shaderType == GL_VERTEX_SHADER)
				{
					loadedVert = true;
				}

				if (shaderType == GL_FRAGMENT_SHADER)
				{
					loadedFrag = true;
				}

				// Open the shader file
				ifstream shaderFile(shdrPath);
				if (shaderFile.is_open( ))
				{
					std::string shaderSrcStr((std::istreambuf_iterator<char>(shaderFile)),
											 std::istreambuf_iterator<char>( ));
					shaderFile.close( );

					GLuint		  shaderID  = glCreateShader(shaderType);
					const GLchar* shaderSrc = shaderSrcStr.c_str( );
					glShaderSource(shaderID, 1, &shaderSrc, NULL);
					glCompileShader(shaderID);

					GLint compileStatus;
					glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
					if (compileStatus == GL_TRUE)
					{
						shaderIDs.push_back(shaderID);
					}
					else
					{
						GLint logLen;
						glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);

						bool printLog = false;

						std::string shaderTypeStr = allShaderTypes[shaderType];
						std::cerr << "ERROR!::Shader[" << shaderTypeStr << "]Failed to compile "
								  << shaderTypeStr << "shader";
						if (logLen > 0)
						{
							infoLog.resize(static_cast<std::size_t>(logLen));
							GLsizei length;
							glGetShaderInfoLog(shaderID, logLen, &length, &(infoLog[0]));

							if (length > 0)
							{
								printLog = true;
								std::cerr
									<< ": " << std::endl
									<< "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
									<< &(infoLog[0]) << std::endl
									<< std::endl;
							}
						}

						if (!printLog)
						{
							std::cerr << "!" << std::endl << std::endl;
						}

						glDeleteShader(shaderID);
					}
				}
				else
				{
					success					  = false;
					std::string shaderTypeStr = allShaderTypes[shaderType];
					std::cerr << "ERROR!::Shader[" << shaderTypeStr
							  << "] -- Failed to open shader file: " << shdrPath << std::endl
							  << std::endl;
				}
			}
		}

		if (success && loadedVert && loadedFrag)
		{
			programID = glCreateProgram( );

			for (auto& shdr : shaderIDs)
			{
				glAttachShader(programID, shdr);
			}

			glLinkProgram(programID);

			GLint linkStatus;
			glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

			if (linkStatus == GL_FALSE)
			{
				success = false;
				GLint logLen;
				glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);

				bool printLog = false;

				std::cerr << "ERROR!::Shader[program] Failed to link shader program";
				if (logLen > 0)
				{
					infoLog.resize(static_cast<std::size_t>(logLen));
					GLsizei length;
					glGetProgramInfoLog(programID, logLen, &length, &(infoLog[0]));

					if (length > 0)
					{
						printLog = true;
						std::cerr << ": " << std::endl
								  << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
								  << &(infoLog[0]) << std::endl
								  << std::endl;
					}
				}

				if (!printLog)
				{
					std::cerr << "!" << std::endl << std::endl;
				}
			}
		}
		else
		{
			success = false;
			std::cerr << "ERROR!::Shader[program] -- Vertex and/or fragment shader not found! "
						 "Note: Vertex AND Fragment shaders are REQUIRED!";
		}

		for (auto& shdr : shaderIDs)
		{
			glDeleteShader(shdr);
		}

		if (success)
		{
			GLint numUniforms;
			glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);

			static const GLsizei nameBufSz = 512;
			GLsizei				 nameLen;
			GLchar				 nameBuf[nameBufSz];

			GLint  size;
			GLenum type;


			for (GLint i = 0; i < numUniforms; ++i)
			{
				glGetActiveUniform(programID, static_cast<GLuint>(i), nameBufSz, &nameLen, &size,
								   &type, nameBuf);

				std::string name(nameBuf, static_cast<std::size_t>(nameLen));
				UniformData dat;
				dat.location   = i;
				dat.type	   = type;
				uniforms[name] = dat;
			}
		}

		return success;
	}

	// Bind this shader for use
	void Shader::BindShader( )
	{
		if (glIsProgram(programID) && currentProgramID != programID)
		{
			glUseProgram(programID);
			currentProgramID = programID;
		}
	}

	// If this shader is bound, call 'glUseProgram(0)'
	void Shader::UnbindShader( )
	{
		if (currentProgramID == programID)
		{
			glUseProgram(0);
			currentProgramID = 0;
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Uniform Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	void Shader::Uniform1i(const std::string& uniName, GLint v0)
	{
		if (currentProgramID == programID && uniforms.count(uniName))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_INT) ||
						  (type >= GL_SAMPLER_1D && type <= GL_UNSIGNED_INT_ATOMIC_COUNTER);
			if ((loc >= 0) && typeOK)
			{
				glUniform1i(loc, v0);
			}
		}
	}

	void Shader::Uniform1f(const std::string& uniName, GLfloat v0)
	{
		if (currentProgramID == programID && uniforms.count(uniName))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT);
			if ((loc >= 0) && typeOK)
			{
				glUniform1f(loc, v0);
			}
		}
	}

	void Shader::Uniform2f(const std::string& uniName, GLfloat v0, GLfloat v1)
	{
		if (currentProgramID == programID && uniforms.count(uniName))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_VEC2);
			if ((loc >= 0) && typeOK)
			{
				glUniform2f(loc, v0, v1);
			}
		}
	}

	void Shader::Uniform3f(const std::string& uniName, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		if (currentProgramID == programID && uniforms.count(uniName))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_VEC3);
			if ((loc >= 0) && typeOK)
			{
				glUniform3f(loc, v0, v1, v2);
			}
		}
	}

	void Shader::Uniform4f(const std::string& uniName, GLfloat v0, GLfloat v1, GLfloat v2,
						   GLfloat v3)
	{
		if (currentProgramID == programID && uniforms.count(uniName))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_VEC4);
			if ((loc >= 0) && typeOK)
			{
				glUniform4f(loc, v0, v1, v2, v3);
			}
		}
	}

	void Shader::Uniform2f(const std::string& uniName, const glm::vec2& v)
	{
		Uniform2f(uniName, v.x, v.y);
	}

	void Shader::Uniform2fv(const std::string&						   uniName,
							const boost::container::vector<glm::vec2>& v)
	{
		std::string name = uniName;
		if (!boost::regex_search(name, boost::regex("\\[\\d\\]")))
		{
			name = name + "[0]";
		}

		if (currentProgramID == programID && uniforms.count(name))
		{
			const UniformData& dat  = uniforms[name];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_VEC2);
			if ((loc >= 0) && typeOK)
			{
				static const GLuint				  numComps = GLuint(2);
				boost::container::vector<GLfloat> data;
				data.reserve(v.size( ) * numComps);

				for (GLuint i = 0; i < v.size( ); ++i)
				{
					for (GLuint c = 0; c < numComps; ++c)
					{
						data.push_back(v[i][c]);
					}
				}

				glUniform2fv(loc, v.size( ), &(data[0]));
			}
		}
	}


	void Shader::Uniform3f(const std::string& uniName, const glm::vec3& v)
	{
		Uniform3f(uniName, v.x, v.y, v.z);
	}

	void Shader::Uniform3fv(const std::string&						   uniName,
							const boost::container::vector<glm::vec3>& v)
	{
		std::string name = uniName;
		if (!boost::regex_search(name, boost::regex("\\[\\d\\]")))
		{
			name = name + "[0]";
		}

		if (currentProgramID == programID && uniforms.count(name))
		{
			const UniformData& dat  = uniforms[name];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_VEC3);
			if ((loc >= 0) && typeOK)
			{
				static const GLuint				  numComps = GLuint(3);
				boost::container::vector<GLfloat> data;
				data.reserve(v.size( ) * numComps);

				for (GLuint i = 0; i < v.size( ); ++i)
				{
					for (GLuint c = 0; c < numComps; ++c)
					{
						data.push_back(v[i][c]);
					}
				}

				glUniform3fv(loc, v.size( ), &(data[0]));
			}
		}
	}


	void Shader::Uniform4f(const std::string& uniName, const glm::vec4& v)
	{
		Uniform4f(uniName, v.x, v.y, v.z, v.w);
	}

	void Shader::Uniform4fv(const std::string&						   uniName,
							const boost::container::vector<glm::vec4>& v)
	{
		std::string name = uniName;
		if (!boost::regex_search(name, boost::regex("\\[\\d\\]")))
		{
			name = name + "[0]";
		}

		if (currentProgramID == programID && uniforms.count(name))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = dat.location;
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_VEC4);
			if ((loc >= 0) && typeOK)
			{
				static const GLuint				  numComps = GLuint(4);
				boost::container::vector<GLfloat> data;
				data.reserve(v.size( ) * numComps);

				for (GLuint i = 0; i < v.size( ); ++i)
				{
					for (GLuint c = 0; c < numComps; ++c)
					{
						data.push_back(v[i][c]);
					}
				}

				glUniform4fv(loc, v.size( ), &(data[0]));
			}
		}
	}




	void Shader::UniformMat4f(const std::string& uniName, const glm::mat4& m)
	{
		if (currentProgramID == programID && uniforms.count(uniName))
		{
			const UniformData& dat  = uniforms[uniName];
			GLint			   loc  = glGetUniformLocation(programID, uniName.c_str( ));
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_MAT4);
			if ((loc >= 0) && typeOK)
			{
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
			}
		}
	}

	void Shader::UniformMat4fv(const std::string&						  uniName,
							   const boost::container::vector<glm::mat4>& m)
	{
		std::string name = uniName;
		if (!boost::regex_search(name, boost::regex("\\[\\d\\]")))
		{
			name = name + "[0]";
		}

		if (currentProgramID == programID && uniforms.count(name))
		{
			static const GLuint				  numComps = GLuint(4);
			boost::container::vector<GLfloat> data;
			data.reserve(m.size( ) * numComps * numComps);

			for (GLuint i = 0; i < m.size( ); ++i)
			{
				for (GLuint j = 0; j < numComps; ++j)
				{
					for (GLuint k = 0; k < numComps; ++k)
					{
						data.push_back(m[i][j][k]);
					}
				}
			}

			const UniformData& dat  = uniforms[name];
			GLint			   loc  = glGetUniformLocation(programID, uniName.c_str( ));
			GLenum			   type = dat.type;

			bool typeOK = (type == GL_FLOAT_MAT4);
			if ((loc >= 0) && typeOK)
			{
				glUniformMatrix4fv(loc, m.size( ), GL_FALSE, &(data[0]));
			}
		}
	}
}
