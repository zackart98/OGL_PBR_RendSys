#include <rendsys/gfx/VertexArray.hpp>

#include <iostream>

namespace rendsys
{

	// Default ctor
	VertexArray::VertexArray( )
	{
		vaoID = 0;
		eboID = 0;
		count = -1;
		vbos  = boost::container::vector<VertexBuffer*>( );

		glGenVertexArrays(1, &vaoID);
	}
	
	// Dtor
	VertexArray::~VertexArray( )
	{
		if (vaoID != 0)
		{
			glDeleteVertexArrays(1, &vaoID);
			vaoID = 0;
		}

		if (eboID != 0)
		{
			glDeleteBuffers(1, &eboID);
			eboID = 0;
		}

		if (!vbos.empty( ))
		{
			for (auto& vbo : vbos)
			{
				if (vbo != nullptr)
				{
					delete vbo;
					vbo = nullptr;
				}
			}
			vbos.clear( );
		}
	}

	// Add a new vertex buffer; Return the index of the new vertex buffer
	GLsizei VertexArray::CreateVertexBuffer( )
	{
		VertexBuffer* vbo = new VertexBuffer( );
		vbos.push_back(vbo);
		return static_cast<GLsizei>(vbos.size( ) - 1);
	}

	void VertexArray::BindVAO()
	{
		glBindVertexArray(vaoID);
	}
	
	// Get a vertex buffer by index
	VertexBuffer* VertexArray::GetVertexBuffer(GLsizei vboIdx)
	{
		if (vboIdx >= 0 && vboIdx < static_cast<GLsizei>(vbos.size( )))
		{
			return vbos[static_cast<std::size_t>(vboIdx)];
		}

		return nullptr;
	}

	// Setup a vertex attribute for this vertex array
	void VertexArray::SetupVertexAttribute(GLuint index, GLint size, GLenum type,
										   GLboolean normalized, GLsizei stride,
										   const GLvoid* pointer, bool isInstanced, bool isMatrix)
	{
		if (isMatrix)
		{
			GLintptr startOffset = reinterpret_cast<GLintptr>(pointer);
			GLintptr colOffset   = (isMatrix) ? (stride / size) : (0);


			GLuint count = (isMatrix) ? (static_cast<GLuint>(size)) : (1);
			for (GLuint i = 0; i < count; ++i)
			{
				SetupVertexAttribImpl(index + i, size, type, normalized, stride,
									  RSYS_BUFR_OFST((startOffset + (colOffset * i))), isInstanced);
			}
		}
		else
		{
			SetupVertexAttribImpl(index, size, type, normalized, stride, pointer, isInstanced);
		}
	}

	// Sets the number of vertices/elements to draw
	void VertexArray::SetCount(GLsizei newCount)
	{
		if (newCount >= 0)
		{
			count = newCount;
		}
	}

	// Draw the vertex array
	void VertexArray::DrawVAO(GLenum mode)
	{

		if (count > 0)
		{
			GLint lastVAO;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVAO);

			glBindVertexArray(vaoID);

			if (eboID != 0)
			{
				glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(mode, 0, count);
			}

			glBindVertexArray(GLuint(lastVAO));
		}
	}

	void VertexArray::SetupVertexAttribImpl(GLuint index, GLint size, GLenum type,
											GLboolean normalized, GLsizei stride,
											const GLvoid* pointer, bool isInstanced)
	{
		glEnableVertexAttribArray(index);

#if defined(RSYS_VAO_BINDING_POINTS)

		if (stride <= 0)
		{
			GLsizeiptr typeSz;
			if (type == GL_BYTE)
			{
				typeSz = sizeof(GLbyte);
			}
			else if (type == GL_UNSIGNED_BYTE)
			{
				typeSz = sizeof(GLubyte);
			}
			else if (type == GL_SHORT)
			{
				typeSz = sizeof(GLshort);
			}
			else if (type == GL_UNSIGNED_SHORT)
			{
				typeSz = sizeof(GLushort);
			}
			else if (type == GL_INT)
			{
				typeSz = sizeof(GLint);
			}
			else if (type == GL_UNSIGNED_INT)
			{
				typeSz = sizeof(GLuint);
			}
			else if (type == GL_HALF_FLOAT)
			{
				typeSz = sizeof(GLhalf);
			}
			else if (type == GL_FLOAT)
			{
				typeSz = sizeof(GLfloat);
			}
			else if (type == GL_DOUBLE)
			{
				typeSz = sizeof(GLdouble);
			}
			else if (type == GL_FIXED)
			{
				typeSz = sizeof(GLfixed);
			}
			else if (type == GL_INT_2_10_10_10_REV)
			{
				typeSz = sizeof(GLuint);
			}
			else if (type == GL_UNSIGNED_INT_2_10_10_10_REV)
			{
				typeSz = sizeof(GLuint);
			}
#if defined(GL_VERSION_4_4) && (GL_VERSION_4_4 == 1)
			else if (type == GL_UNSIGNED_INT_10F_11F_11F_REV)
			{
				typeSz = sizeof(GLuint);
			}
#endif
			else
			{
				std::cerr << "ERROR!::VertexArray::Invalid_Value_For_Vertex_Attribute_Type[type = "
						  << type << "]" << std::endl
						  << std::endl;
				return;
			}

			stride = static_cast<GLsizei>(typeSz) * static_cast<GLsizei>(size);
			if (stride <= 0)
			{
				std::cerr << "ERROR!::VertexArray::Could_Mot_Calculate_Stride[stride = " << stride
						  << "]" << std::endl
						  << std::endl;
				return;
			}
		}

		glVertexAttribFormat(index, size, type, normalized, 0);
		glVertexAttribBinding(index, index);

		GLint vboID;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vboID);

		if (vboID == 0)
		{
			std::cerr << "ERROR!::VertexArray::No_Buffer_Bound_To___GL_ARRAY_BUFFER" << std::endl
					  << std::endl;
			return;
		}

		GLintptr offset = reinterpret_cast<GLintptr>(pointer);
		glBindVertexBuffer(index, vboID, offset, stride);

		if (isInstanced)
		{
			glVertexBindingDivisor(index, 1);
		}

#else
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		if (isInstanced)
		{
			glVertexAttribDivisor(index, 1);
		}
#endif
	}
}
