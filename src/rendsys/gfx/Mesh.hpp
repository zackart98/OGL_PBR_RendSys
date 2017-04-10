#ifndef RENDSYS_MESH_HPP_INC
#define RENDSYS_MESH_HPP_INC

#include <rendsys/gfx/GL.hpp>

#include <glm/glm.hpp>

namespace rendsys
{
	namespace detail
	{
		
		// A vertex on a static mesh
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
		
	}	
}

#endif
