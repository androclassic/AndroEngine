#include "GL/glew.h"
#include "Log.h"
#include "Mesh.h"
#include "Engine.h"
#include <fstream>

TakeTwo::Mesh::Mesh() :
    mVBO(0), mIBO(0)
{

}

TakeTwo::Mesh::~Mesh()
{
    Release();
}

void TakeTwo::Mesh::Render()
{
	PD_CommandBuffer* cb = Engine::GetInstance()->GetCommandBuffer();


    //Bind vertex data
	cb->BindVertexBuffer(mVBO, mAttribsUsed);

 
	if (mIndices.size())
	{
		cb->BindIndexBuffer(mIBO);
		cb->DrawIndexed(static_cast<int>(mIndices.size()), 0, 0);
	}
	else
	{
		cb->Draw(mVertices.size(), 0);
	}
}

void TakeTwo::Mesh::Release()
{
	PD_CommandBuffer* cb = Engine::GetInstance()->GetCommandBuffer();
	cb->BindIndexBuffer(nullptr);
	cb->BindVertexBuffer(nullptr, mAttribsUsed ); 

	if(mVBO)
	{
		delete mVBO;
		mVBO = nullptr;
	}

	if (mIBO)
	{
		delete mIBO;
		mIBO = nullptr;
	}
}

void TakeTwo::Mesh::Setup()
{
    Release();
   
	mVBO = Engine::GetInstance()->GetFactory()->CreateVertexBuffer(mVertices.size() * sizeof(Vertex), &mVertices[0]);
	if (mIndices.size())
	{
		mIBO = Engine::GetInstance()->GetFactory()->CreateIndexBuffer(mIndices.size() * sizeof(unsigned int), &mIndices[0]);
	}
}

void TakeTwo::Mesh::SetupFaces()
{
	m_faces.clear();

	for (int i = 0; i < mIndices.size() - 2; i++)
	{
		Vertex v1 = mVertices[mIndices[i]];
		Vertex v2 = mVertices[mIndices[i + 1]];
		Vertex v3 = mVertices[mIndices[i + 2]];

		andro::Triangle t;
		t.P1 = andro::Vector3(v1.position.x, v1.position.y, v1.position.z);
		t.P2 = andro::Vector3(v2.position.x, v2.position.y, v2.position.z);
		t.P3 = andro::Vector3(v3.position.x, v3.position.y, v3.position.z);
		m_faces.push_back(t);
	}

}
