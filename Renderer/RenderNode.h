#pragma once

#include "Node.h"
#include "RenderObject.h"
#include <memory>

namespace TakeTwo
{
    class RenderNode : public Node
    {
    public:
        RenderNode(RenderObject* pRenderObject = nullptr);
        RenderNode(const RenderNode&) = delete;
        RenderNode& operator= (const RenderNode&) = delete;
        RenderNode(RenderNode&&);
        RenderNode& operator= (RenderNode&&);
        ~RenderNode();

        void SetRenderObject(RenderObject* pRenderObject);
		RenderObject& GetRenderObject();

        void ApplyTransformation(const std::string& pUniformName);
    private:
		RenderObject* mRenderObject;
    };
}
