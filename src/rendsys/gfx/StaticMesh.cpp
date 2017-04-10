#include <rendsys/gfx/StaticMesh.hpp>

#include <cstddef>

namespace rendsys
{
	// Default ctor
	StaticMesh::StaticMesh( )
	{
		vao		 = nullptr;
		numInsts = -1;
		vertVboIdx = -1;
		mmatVboIdx = -1;
	}


	// Construct from vertices and indices
	StaticMesh::StaticMesh(const boost::container::vector<StaticMeshVertex>& verts,
			   const boost::container::vector<GLuint>&			 idxs)
	{
		vao		 = nullptr;
		numInsts = -1;
		vertVboIdx = -1;
		mmatVboIdx = -1;
		SetupMesh(verts, idxs);
	}

	// Dtor
	StaticMesh::~StaticMesh( )
	{
		if (vao != nullptr)
		{
			delete vao;
			vao = nullptr;
		}
		numInsts = -1;
		vertVboIdx = -1;
		mmatVboIdx = -1;
	}
	
	// Set the model matrix instance data
	void StaticMesh::SetModelMats(boost::container::vector<glm::mat4>& modelMats)
	{
		VertexBuffer* mmatVbo = vao->GetVertexBuffer(mmatVboIdx);
		
		if (mmatVbo != nullptr)
		{
			if (modelMats.size() > GLuint(numInsts))
			{
				mmatVbo->BufferData(modelMats, GL_DYNAMIC_DRAW);
			}
			else
			{
				mmatVbo->BufferSubData(modelMats, 0);
			}
			numInsts = modelMats.size();
		}
	}
	
	// Draw the mesh using instanced renedering
	void StaticMesh::DrawMesh(GLenum primMode)
	{
		if (numInsts > 0)
		{
			vao->DrawVAOInst(primMode, numInsts);
		}
	}


	// Setup the mesh vertex array
	void StaticMesh::SetupMesh(const boost::container::vector<StaticMeshVertex>& verts,
							   const boost::container::vector<GLuint>&			 idxs)
	{
		vao = new VertexArray( );
		vao->BindVAO( );

		vertVboIdx = vao->CreateVertexBuffer( );
		VertexBuffer* vertVbo	= vao->GetVertexBuffer(vertVboIdx);

		mmatVboIdx = vao->CreateVertexBuffer( );
		VertexBuffer* mmatVbo	= vao->GetVertexBuffer(mmatVboIdx);

		if (vertVbo != nullptr && mmatVbo != nullptr)
		{
			vao->AddEBO(idxs);

			vertVbo->BufferData(verts, GL_STATIC_DRAW);
			vertVbo->BindVBO( );
			vao->SetupVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, (sizeof(StaticMeshVertex)),
									  RSYS_BUFR_OFST(offsetof(StaticMeshVertex, vPosition)));
			vao->SetupVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, (sizeof(StaticMeshVertex)),
									  RSYS_BUFR_OFST(offsetof(StaticMeshVertex, vNormal)));
			vao->SetupVertexAttribute(2, 3, GL_FLOAT, GL_FALSE, (sizeof(StaticMeshVertex)),
									  RSYS_BUFR_OFST(offsetof(StaticMeshVertex, vTangent)));
			vao->SetupVertexAttribute(3, 2, GL_FLOAT, GL_FALSE, (sizeof(StaticMeshVertex)),
									  RSYS_BUFR_OFST(offsetof(StaticMeshVertex, vTexCoords)));

			boost::container::vector<glm::mat4> mmats = { glm::mat4(1.0f) };
			mmatVbo->BufferData(mmats, GL_DYNAMIC_DRAW);
			mmatVbo->BindVBO( );
			vao->SetupVertexAttribute(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
									  RSYS_BUFR_OFST(0), true, true);
		}

		vao->UnbindVAO( );
		
		numInsts = 1;
	}
}
