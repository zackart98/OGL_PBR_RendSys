#ifndef RENDSYS_STATICMODEL_HPP_INC
#define RENDSYS_STATICMODEL_HPP_INC

#include <rendsys/gfx/StaticMesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/container/vector.hpp>

namespace rendsys
{

	// A 3D model made up of zero or more static meshes
	class StaticModel
	{
	  public:

		// Default ctor
		StaticModel( );
		
		// Load the model from a file
		explicit StaticModel(const std::string& modelPath);
		
		// Dtor
		~StaticModel( );
		
		
		
		// Draw the static model
		void DrawModel(GLenum primMode = GL_TRIANGLES);
		
		// Set the model matrix instance data
		void SetModelMats(boost::container::vector<glm::mat4>& modelMats);
		

		// Load the model from a file
		bool LoadModel(const std::string& modelPath);
		
	  private:
		
		// All the meshes in this model
		boost::container::vector<StaticMesh*> meshes;
		
		
		// Process an Assimp node and produce 'StaticMesh' objects for all
		// the nodes it contains
		void ProcessAiNode(aiNode* node, const aiScene* scene);

		// Process an Assimp mesh and produce a 'StaticMesh' object from it
		StaticMesh* ProcessAiMesh(aiMesh* mesh, const aiScene* scene);
	};
}

#endif
