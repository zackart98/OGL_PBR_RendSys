#ifndef RENDSYS_VERTEXBUFFER_HPP_INC
#define RENDSYS_VERTEXBUFFER_HPP_INC

#include <rendsys/gfx/GL.hpp>

#include <boost/container/vector.hpp>

#include <rendsys/Config.hpp>

namespace rendsys
{

	// Wrapper class for an OpenGL vertex buffer object (VBO)
	class VertexBuffer
	{
	  public:
		
		// Ctor
		VertexBuffer( )
		{
			glGenBuffers(1, &vboID);
			vboSize = 0;
			dataUsage = GL_STATIC_DRAW;
		}
		
		// Dtor
		~VertexBuffer()
		{
			if (vboID != 0)
			{
				glDeleteBuffers(1, &vboID);
			}
		}
		
		
		// Fill the vertex buffer with data by calling 'glBufferData'
		template <typename VertexT>
		void BufferData(const boost::container::vector<VertexT>& data, GLenum usage = GL_STATIC_DRAW)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			{
				glBufferData(GL_ARRAY_BUFFER, (sizeof(VertexT) * data.size()), &(data[0]), usage);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			vboSize = (sizeof(VertexT) * data.size());
			dataUsage = usage;
		}
		
		template <typename VertexT>
		void BufferSubData(const boost::container::vector<VertexT>& data, GLsizeiptr offset)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			GLsizeiptr reqSz = (sizeof(VertexT) * data.size()) + offset;
			if (reqSz > vboSize)
			{
				glBufferData(GL_ARRAY_BUFFER, reqSz, NULL, GL_DYNAMIC_DRAW);
				vboSize = reqSz;
			}
			
			glBufferSubData(GL_ARRAY_BUFFER, offset, (sizeof(VertexT) * data.size()), &(data[0]));
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
				
		}
		
		// Bind the vertex buffer object to 'GL_ARRAY_BUFFER'
		void BindVBO()
		{
			if (glIsBuffer(vboID))
			{
				glBindBuffer(GL_ARRAY_BUFFER, vboID);
			}
		}
		
		// Unbind the vertex buffer object from 'GL_ARRAY_BUFFER'
		void UnbindVBO()
		{
			GLint currentVBO;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVBO);
			if (GLuint(currentVBO) == vboID)
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		
#if defined(GL_VERSION_4_3) && (GL_VERSION_4_3 == 1)
		// Bind the vertex buffer object to 'GL_ARRAY_BUFFER'
		void BindVBOToVAO(GLuint bindingIndex, GLintptr offset, GLintptr stride)
		{
			if (glIsBuffer(vboID) && (stride >= 0) && (offset >= 0))
			{
				glBindVertexBuffer(bindingIndex, vboID, offset, stride);
			}
		}
#endif

	  private:
		// Internal OpenGL vertex buffer object name
		GLuint vboID;

		// The size of the buffer
		GLsizeiptr vboSize;
		
		// The data usage type of the VBO
		GLenum dataUsage;
	};
}

#endif
