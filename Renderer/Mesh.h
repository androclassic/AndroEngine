#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "OGL_Platform.h"

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

    private:
        PDBuffer* mVBO;
        PDBuffer* mIBO;

        unsigned short mIndicesType;

        std::vector<unsigned int>   mIndices;
        std::vector<Vertex>         mVertices;

        std::vector<unsigned int> mAttribsUsed;
    };
}
