#ifndef RENDSYS_STATICMESH_HPP_INC
#define RENDSYS_STATICMESH_HPP_INC

#include <rendsys/gfx/VertexArray.hpp>

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
		
		StaticMesh();
		
	  private:
		
		// The vertex array that holds the model data
		VertexArray* vao;
		
		// The number of instances to draw
		GLsizei numInsts;
		
	};
}

#endif
