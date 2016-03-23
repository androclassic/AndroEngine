#include "Transform.h"
#include "glm/gtx/transform.hpp"
#include <algorithm>

TakeTwo::Transform::Transform(Transform* const pParent)
        : mWorldTransform(glm::mat4()), mPosition(glm::vec3(0.0f)), mScale(glm::vec3(1.0f)), mRotation(glm::quat()), mDirty(true), mParent(pParent)
{

}

TakeTwo::Transform::~Transform()
{

}

void TakeTwo::Transform::SetParent(TakeTwo::Transform* const pParent)
{
    if(mParent)
    {
        mParent->RemoveChild(this);
    }

    if(pParent)
    {
        pParent->AddChild(this);
    }
}

const TakeTwo::Transform* TakeTwo::Transform::GetParent() const
{
    return mParent;
}

void TakeTwo::Transform::AddChild(Transform* const pChild)
{
    mChildren.push_back(pChild);
    if(pChild->GetParent())
    {
        pChild->SetParent(nullptr);
    }
    pChild->mParent = this;
}

void TakeTwo::Transform::RemoveChild(TakeTwo::Transform* const pChild)
{
    mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), pChild), mChildren.end());
    pChild->mParent = nullptr;
}

const std::vector<TakeTwo::Transform*>& TakeTwo::Transform::GetChildren() const
{
    return mChildren;
}


glm::mat4 TakeTwo::Transform::GetTransformMatrix()
{
    UpdateIfDirty();
    return mWorldTransform;
}

glm::mat4 TakeTwo::Transform::GetLocalTransformMatrix() const
{
    return glm::translate(mPosition) * glm::mat4_cast(mRotation) * glm::scale(mScale);
}

void TakeTwo::Transform::SetPosition(const glm::vec3& pPosition)
{
    SetDirty();
    mPosition = pPosition;
}

void TakeTwo::Transform::SetScale(const glm::vec3& pScale)
{
    SetDirty();
    mScale = pScale;
}

void TakeTwo::Transform::SetRotation(const glm::quat& pRotation)
{
    SetDirty();
    mRotation = pRotation;
}

glm::vec3 TakeTwo::Transform::GetPosition() const
{
    return mPosition;
}

glm::vec3 TakeTwo::Transform::GetScale() const
{
    return mScale;
}

glm::quat TakeTwo::Transform::GetRotation() const
{
    return mRotation;
}

void TakeTwo::Transform::UpdateIfDirty()
{
    if(mDirty)
    {
        if(mParent)
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

void TakeTwo::Transform::SetDirty()
{
    mDirty = true;
    for(auto child : mChildren)
    {
        child->SetDirty();
    }
}
