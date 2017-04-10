#include <rendsys/gfx/StaticModel.hpp>

#include <iostream>


namespace rendsys
{
	// Default ctor
	StaticModel::StaticModel( )
	{
		meshes = boost::container::vector<StaticMesh*>( );
	}

	// Load the model from a file
	StaticModel::StaticModel(const std::string& modelPath)
	{
		meshes = boost::container::vector<StaticMesh*>( );
		LoadModel(modelPath);
	}

	// Dtor
	StaticModel::~StaticModel( )
	{
		for (auto& mesh : meshes)
		{
			delete mesh;
			mesh = nullptr;
		}
		meshes.clear( );
	}



	// Load the model from a file
	bool StaticModel::LoadModel(const std::string& modelPath)
	{
		Assimp::Importer reader;

		const aiScene* scene =
			reader.ReadFile(modelPath, (aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
										aiProcess_GenNormals | aiProcess_Triangulate));

		if (scene != nullptr && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) &&
			scene->mRootNode != nullptr)
		{
			ProcessAiNode(scene->mRootNode, scene);
			return true;
		}
		std::cerr << "ERROR!::Assimp::" << reader.GetErrorString( ) << std::endl << std::endl;
		return false;
	}


	// Process an Assimp node and produce 'StaticMesh' objects for all
	// the nodes it contains
	void StaticModel::ProcessAiNode(aiNode* node, const aiScene* scene)
	{
		// Create StaticMesh objects
		for (GLuint m = 0; m < node->mNumMeshes; ++m)
		{
			aiMesh*		tmpMesh		= scene->mMeshes[node->mMeshes[m]];
			StaticMesh* createdMesh = ProcessAiMesh(tmpMesh, scene);
			meshes.push_back(createdMesh);
		}

		// Recursion: Process children
		for (GLuint c = 0; c < node->mNumChildren; ++c)
		{
			ProcessAiNode(node->mChildren[c], scene);
		}
	}

	// Process an Assimp mesh and produce a 'StaticMesh' object from it
	StaticMesh* StaticModel::ProcessAiMesh(aiMesh* mesh, const aiScene* scene)
	{
		boost::container::vector<StaticMeshVertex> verts;
		boost::container::vector<GLuint>		   idxs;

		verts.reserve(mesh->mNumVertices);
		for (GLuint v = 0; v < mesh->mNumVertices; ++v)
		{
			StaticMeshVertex vert;

			vert.vPosition.x = mesh->mVertices[v].x;
			vert.vPosition.y = mesh->mVertices[v].y;
			vert.vPosition.z = mesh->mVertices[v].z;

			vert.vNormal.x = mesh->mNormals[v].x;
			vert.vNormal.y = mesh->mNormals[v].y;
			vert.vNormal.z = mesh->mNormals[v].z;

			if (mesh->mTangents)
			{
				vert.vTangent.x = mesh->mTangents[v].x;
				vert.vTangent.y = mesh->mTangents[v].y;
				vert.vTangent.z = mesh->mTangents[v].z;
			}
			else
			{
				vert.vTangent = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				vert.vTexCoords.x = mesh->mTextureCoords[0][v].x;
				vert.vTexCoords.y = mesh->mTextureCoords[0][v].y;
			}
			else
			{
				vert.vTexCoords = glm::vec2(0.0f);
			}

			verts.push_back(vert);
		}

		// Process indices
		for (GLuint f = 0; f < mesh->mNumFaces; ++f)
		{
			aiFace face = mesh->mFaces[f];
			for (GLuint i = 0; i < face.mNumIndices; ++i)
			{
				idxs.push_back(face.mIndices[i]);
			}
		}

		return new StaticMesh(verts, idxs);
	}
}
