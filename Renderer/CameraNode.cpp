#include "CameraNode.h"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <GL/glew.h>

TakeTwo::CameraNode::CameraNode(TakeTwo::CameraType pCameraType)
        : mCameraType(pCameraType), mNearPlane(0.0f), mFarPlane(100.0f), mFOV(45.0f), mAspectRatio(4.0f/3.0f),
          mLeft(-1.0f), mRight(1.0f), mBottom(-1.0f), mTop(1.0f), mProjection(0.0f)
{

}

TakeTwo::CameraNode::~CameraNode()
{

}

void TakeTwo::CameraNode::SetClearColor(const glm::vec4& pClearColor)
{
    mClearColor = pClearColor;
    glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
}

glm::vec4 TakeTwo::CameraNode::GetClearColor() const
{
    return mClearColor;
}

void TakeTwo::CameraNode::SetAngleAxis(const glm::vec4& pAngleAxis)
{
    SetRotation(glm::angleAxis(pAngleAxis.x, glm::vec3(pAngleAxis.y, pAngleAxis.z, pAngleAxis.w)));
}

void TakeTwo::CameraNode::SetAngleAxis(afloat pAngle, const glm::vec3& pAxis)
{
	SetRotation(glm::angleAxis((float)pAngle, pAxis));
}

glm::vec4 TakeTwo::CameraNode::GetAngleAxis() const
{
    auto quat = GetRotation();
    return glm::vec4(glm::angle(quat), glm::axis(quat));
}

afloat TakeTwo::CameraNode::GetAngle() const
{
    return glm::angle(GetRotation());
}

glm::vec3 TakeTwo::CameraNode::GetAxis() const
{
    return glm::axis(GetRotation());
}

void TakeTwo::CameraNode::Rotate(const glm::vec4& pAngleAxis)
{
    SetRotation(
        GetRotation() * glm::angleAxis(pAngleAxis.x, glm::vec3(pAngleAxis.y, pAngleAxis.z, pAngleAxis.w))
    );
}

void TakeTwo::CameraNode::Rotate(afloat pAngle, const glm::vec3& pAxis)
{
	SetRotation(GetRotation() * glm::angleAxis((float)pAngle, pAxis));
}

void TakeTwo::CameraNode::LookAt(const glm::vec3& pLookAt, const glm::vec3& pUp)
{
    SetRotation(
        glm::toQuat(
            glm::lookAt(
                GetPosition(),
                pLookAt,
                pUp
            )
        )
    );
}

glm::vec3 TakeTwo::CameraNode::GetFrontDir() 
{
    glm::mat4& mat = GetTransformMatrix();
	return glm::vec3(-mat[2][0], -mat[2][1], -mat[2][2]);
}

glm::vec3 TakeTwo::CameraNode::GetUpDir() 
{
	glm::mat4& mat = GetTransformMatrix();
	return glm::vec3(-mat[1][0], -mat[1][1],-mat[1][2]);
}

glm::vec3 TakeTwo::CameraNode::GetRightDir() 
{
	glm::mat4& mat = GetTransformMatrix();
	return glm::vec3(-mat[0][0], -mat[0][1], -mat[0][2]);
}

void TakeTwo::CameraNode::SetPerspective(afloat pFOV, afloat pAspectRatio, afloat pNearPlane, afloat pFarPlane)
{
    assert(mCameraType == CameraType::PERSPECTIVE);
    mFOV = pFOV;
    mAspectRatio = pAspectRatio;
    mNearPlane = pNearPlane;
    mFarPlane = pFarPlane;

    mProjection = static_cast<glm::mat4>(glm::perspective(pFOV, pAspectRatio, pNearPlane, pFarPlane));
}

void TakeTwo::CameraNode::SetPerspectiveFOV(afloat pFOV, afloat width, afloat height, afloat pNearPlane, afloat pFarPlane)
{
	assert(mCameraType == CameraType::PERSPECTIVE);
	mFOV = pFOV;
	mAspectRatio = width / height;
	mNearPlane = pNearPlane;
	mFarPlane = pFarPlane;

	mProjection = static_cast<glm::mat4>(glm::perspectiveFov(pFOV,  width, height, pNearPlane, pFarPlane));
}
void TakeTwo::CameraNode::SetOrthographic(afloat pLeft, afloat pRight, afloat pTop, afloat pBottom, afloat pNearPlane, afloat pFarPlane)
{
    assert(mCameraType == CameraType::ORTHOGRAPHIC);
    mLeft = pLeft;
    mRight = pRight;
    mBottom = pBottom;
    mTop = pTop;
    mNearPlane = pNearPlane;
    mFarPlane = pFarPlane;

    mProjection = static_cast<glm::mat4>(glm::ortho(pLeft, pRight, pBottom, pTop, pNearPlane, pFarPlane));
}
