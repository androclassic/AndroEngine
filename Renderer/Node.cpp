#include "Node.h"

unsigned int TakeTwo::Node::sIndex = 0;

TakeTwo::Node::Node()
{
    mTransform.reset(new Transform);
    mName = "Node_" + std::to_string(sIndex);
    sIndex++;
}

TakeTwo::Node::Node(Node&& pOther) :
        mTransform(std::move(pOther.mTransform)), mName(std::move(pOther.mName))
{

}

TakeTwo::Node& TakeTwo::Node::operator= (Node&& pOther)
{
    if(this != &pOther)
    {
        mName = std::move(pOther.mName);
        mTransform = std::move(pOther.mTransform);
    }
    return *this;
}

TakeTwo::Node::~Node()
{

}

void TakeTwo::Node::SetName(const std::string& pName)
{
    mName = pName;
}

std::string TakeTwo::Node::GetName() const
{
    return mName;
}

TakeTwo::Transform& TakeTwo::Node::GetTransform()
{
    return *mTransform;
}
