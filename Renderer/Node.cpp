#include "Node.h"
#include "RenderObject.h"
#include "glm/gtx/transform.hpp"

unsigned int TakeTwo::Node::sIndex = 0;

TakeTwo::Node::Node(RenderObject* pRenderObject)
        : mRenderObject(pRenderObject)
		, mWorldTransform(glm::mat4())
		, mPosition(glm::vec3(0.0f))
		, mScale(glm::vec3(1.0f))
		, mRotation(glm::quat())
		, mDirty(true)
		, mParent(nullptr)
{
	
	mName = "Node_" + std::to_string(sIndex);
	sIndex++;

}


TakeTwo::Node::~Node()
{

}

void TakeTwo::Node::SetRenderObject(RenderObject* pRenderObject)
{
    mRenderObject = pRenderObject;
}

TakeTwo::RenderObject& TakeTwo::Node::GetRenderObject()
{
    return *mRenderObject;
}

void TakeTwo::Node::ApplyTransformation(const std::string& pUniformName)
{
    if(mRenderObject)
        mRenderObject->GetMaterial().SetShaderParam(pUniformName, GetTransformMatrix());
}

void TakeTwo::Node::SetName(const std::string& pName)
{
	mName = pName;
}

std::string TakeTwo::Node::GetName() const
{
	return mName;
}

void TakeTwo::Node::SetParent(TakeTwo::Node* const pParent)
{
	if (mParent)
	{
		mParent->RemoveChild(this);
	}

	if (pParent)
	{
		pParent->AddChild(this);
	}
}

const TakeTwo::Node* TakeTwo::Node::GetParent() const
{
	return mParent;
}

void TakeTwo::Node::AddChild(Node* const pChild)
{
	mChildren.push_back(pChild);
	if (pChild->GetParent())
	{
		pChild->SetParent(nullptr);
	}
	pChild->mParent = this;
}

void TakeTwo::Node::RemoveChild(TakeTwo::Node* const pChild)
{
	mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), pChild), mChildren.end());
	pChild->mParent = nullptr;
}

const std::vector<TakeTwo::Node*>& TakeTwo::Node::GetChildren() const
{
	return mChildren;
}


glm::mat4 TakeTwo::Node::GetTransformMatrix()
{
	UpdateIfDirty();
	return mWorldTransform;
}

glm::mat4 TakeTwo::Node::GetLocalTransformMatrix() const
{
	return glm::translate(mPosition) * glm::mat4_cast(mRotation) * glm::scale(mScale);
}

void TakeTwo::Node::SetPosition(const glm::vec3& pPosition)
{
	SetDirty();
	mPosition = pPosition;
}

void TakeTwo::Node::SetScale(const glm::vec3& pScale)
{
	SetDirty();
	mScale = pScale;
}

void TakeTwo::Node::SetRotation(const glm::quat& pRotation)
{
	SetDirty();
	mRotation = pRotation;
}

glm::vec3 TakeTwo::Node::GetPosition() const
{
	return mPosition;
}

glm::vec3 TakeTwo::Node::GetScale() const
{
	return mScale;
}

glm::quat TakeTwo::Node::GetRotation() const
{
	return mRotation;
}

void TakeTwo::Node::UpdateIfDirty()
{
	if (mDirty)
	{
		if (mParent)
		{
			mParent->UpdateIfDirty();
			mWorldTransform = mParent->mWorldTransform * GetLocalTransformMatrix();
		}
		else
		{
			mWorldTransform = GetLocalTransformMatrix();
		}

		mDirty = false;
	}
}

void TakeTwo::Node::SetDirty()
{
	mDirty = true;
	for (auto child : mChildren)
	{
		child->SetDirty();
	}
}