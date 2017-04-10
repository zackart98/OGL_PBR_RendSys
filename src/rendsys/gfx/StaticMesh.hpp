#ifndef RENDSYS_STATICMESH_HPP_INC
#define RENDSYS_STATICMESH_HPP_INC

#include <rendsys/gfx/VertexArray.hpp>


#include <boost/container/vector.hpp>
#include <glm/glm.hpp>

namespace rendsys
{
	// A single vertex on a static mesh
	struct StaticMeshVertex
	{
		// Vertex attribute location: 0
		glm::vec3 vPosition;

		// Vertex attribute location: 1
		glm::vec3 vNormal;

		// Vertex attribute location: 2
		glm::vec3 vTangent;

		// Vertex attribute location: 3
		glm::vec2 vTexCoords;
	};


	// A single node on a static model
	class StaticMesh
	{
	  public:
		// Default ctor
		StaticMesh( );

		// Construct from vertices and indices
		StaticMesh(const boost::container::vector<StaticMeshVertex>& verts,
				   const boost::container::vector<GLuint>&			 idxs);

		// Dtor
		~StaticMesh( );


		// Set the model matrix instance data
		void SetModelMats(boost::container::vector<glm::mat4>& modelMats);
		
		
		
		// Draw the mesh using instanced renedering
		void DrawMesh(GLenum primMode = GL_TRIANGLES);


	  private:
		// The vertex array that holds the model data
		VertexArray* vao;

		// The number of instances to draw
		GLsizei numInsts;


		// Index of the VBO for the vertices
		GLsizei vertVboIdx;

		// Index of the model matrix VBO
		GLsizei mmatVboIdx;

		// Setup the mesh vertex array
		void SetupMesh(const boost::container::vector<StaticMeshVertex>& verts,
					   const boost::container::vector<GLuint>&			 idxs);
	};
}

#endif
