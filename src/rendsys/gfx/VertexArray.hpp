#ifndef RENDSYS_VERTEXARRAY_HPP_INC
#define RENDSYS_VERTEXARRAY_HPP_INC

#include <rendsys/gfx/GL.hpp>

#include <rendsys/gfx/VertexBuffer.hpp>

#include <boost/container/vector.hpp>


#include <rendsys/Config.hpp>
#if GL_VERSION_4_3 != 1 && defined(RSYS_VAO_BINDING_POINTS)
#undef RSYS_VAO_BINDING_POINTS
#endif

namespace rendsys
{

	// Wrapper class for an OpenGL vertex array object (VAO)
	class VertexArray
	{
	  public:
		
		// Default ctor
		VertexArray();
		
		// Dtor
		virtual ~VertexArray();
		
		// Add a new vertex buffer; Return the index of the new vertex buffer
		GLsizei CreateVertexBuffer( );

		// Get a vertex buffer by index
		VertexBuffer* GetVertexBuffer(GLsizei vboIdx);

		// Add an element buffer object (EBO)
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Creates and attaches an element buffer object for this vertex array. The EBO is
		// filled with the indices from the argument 'indices'. SetCount() is called with
		// the 'indices.size()' for 'newCount'.
		void AddEBO(const boost::container::vector<GLuint>& indices);
		
		void BindVAO();
		void UnbindVAO();

		// Setup a vertex attribute for this vertex array
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Parameters are the same as glVertexAttribPointer, with the addition of 'isInstanced' and 'isMatrix'.
		// If 'isMatrix' == true makes this function interpret the attribute's type 'size'x'size' matrix. Thus, it repeats multiple
		// times, to create 'size' vector attributes made up of 'size' components that are are of the type that the enumeration 'type'
		//stands for. If 'isMatrix'  == false, works just like glVertexAttribPointer. 'isInstanced' makes this vertex attribute change
		// per drawn instance (when using instanced drawing functions), instead of per vertex.
		void SetupVertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized,
								  GLsizei stride, const GLvoid* pointer, bool isInstanced = false,
								  bool isMatrix = false);

		
		// Sets the number of vertices/elements to draw
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// If this vertex array has an element buffer object, than the number set here is used to override the index count.
		// Otherwise, the number is used to set the vertex count.
		void SetCount(GLsizei newCount);
		
		// Draw the vertex array
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 'mode' is the primitive rendering mode
		void DrawVAO(GLenum mode);
		
		// Draw the vertex array using instanced rendering
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 'mode' is the primitive rendering mode. 'numInsts' is the number
		// instances to render using instanced rendering
		void DrawVAOInst(GLenum mode, GLsizei numInsts);
		
	  private:
		// Internal OpenGL name for the vertex array object
		GLuint vaoID;

		// Internal OpenGL name for the element buffer object
		GLuint eboID;

		// The number of vertices/indices to draw
		GLsizei count;
		
		// The vertex buffers for this vertex array
		boost::container::vector<VertexBuffer*> vbos;

		void SetupVertexAttribImpl(GLuint index, GLint size, GLenum type, GLboolean normalized,
								   GLsizei stride, const GLvoid* pointer, bool isInstanced);
	};
}

#endif
