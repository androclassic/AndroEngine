#include "RenderNode.h"
#include "RenderObject.h"

TakeTwo::RenderNode::RenderNode(RenderObject* pRenderObject)
        : mRenderObject(pRenderObject)
{

}

TakeTwo::RenderNode::RenderNode(RenderNode&& pOther)
        : Node(std::move(pOther)), mRenderObject(std::move(pOther.mRenderObject))
{

}

TakeTwo::RenderNode& TakeTwo::RenderNode::operator= (RenderNode&& pOther)
{
    if(this != &pOther)
    {
        Node::operator=(std::move(pOther));
        mRenderObject = std::move(pOther.mRenderObject);
    }
    return *this;
}

TakeTwo::RenderNode::~RenderNode()
{

}

void TakeTwo::RenderNode::SetRenderObject(RenderObject* pRenderObject)
{
    mRenderObject = pRenderObject;
}

TakeTwo::RenderObject& TakeTwo::RenderNode::GetRenderObject()
{
    return *mRenderObject;
}

void TakeTwo::RenderNode::ApplyTransformation(const std::string& pUniformName)
{
    if(mRenderObject)
        mRenderObject->GetMaterial().SetShaderParam(pUniformName, mTransform->GetTransformMatrix());
}
