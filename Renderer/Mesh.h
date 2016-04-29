#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "OGL_Platform.h"
#include "../../AndroUtils/Utils/MMath.h"

namespace TakeTwo
{

    class Mesh
    {
    friend class RenderObject;

    public:
        Mesh();
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        ~Mesh();

        void Render();
        void Release();

		
    protected:
        //accessed from RenderObject
        void SetVertices(std::vector<Vertex>&& pVertices) { mVertices = std::move(pVertices); }
        void SetIndices(std::vector<unsigned int>&& pIndices) { mIndices = std::move(pIndices); }
        void SetAttribsUsed(std::vector<unsigned int>&& pAttribsUsed) { mAttribsUsed = std::move(pAttribsUsed); }

        void Setup();

	public:
		unsigned int GetIndexNum() const { return mIndices.size(); }
		andro::Triangle GetTriangle(unsigned int idx)
		{
			ASSERT(idx < mIndices.size()-2 );

			Vertex v1 = mVertices[mIndices[idx]];
			Vertex v2 = mVertices[mIndices[idx + 1]];
			Vertex v3 = mVertices[mIndices[idx + 2 ]];

			andro::Triangle t;
			t.P1 = andro::Vector3(v1.position.x, v1.position.y, v1.position.z);
			t.P2 = andro::Vector3(v2.position.x, v2.position.y, v2.position.z);
			t.P3 = andro::Vector3(v3.position.x, v3.position.y, v3.position.z);
			return t;
		}

    private:
        PDBuffer* mVBO;
        PDBuffer* mIBO;

        unsigned short mIndicesType;

        std::vector<unsigned int>   mIndices;
        std::vector<Vertex>         mVertices;

        std::vector<unsigned int> mAttribsUsed;
    };
}
