#pragma once

#include "Node.h"

namespace TakeTwo
{
    enum class CameraType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    class CameraNode : public Node
    {
    public:
        CameraNode(CameraType pCameraType = CameraType::PERSPECTIVE);
        ~CameraNode(); //empty

        void SetCameraType(CameraType pCameraType) { mCameraType = pCameraType; }
        CameraType GetCameraType() const { return mCameraType; }

        void SetClearColor(const glm::vec4& pClearColor);
        glm::vec4 GetClearColor() const;

        void SetAngleAxis(const glm::vec4& pAngleAxis);
        void SetAngleAxis(afloat pAngle, const glm::vec3& pAxis);
        glm::vec4 GetAngleAxis() const;
        afloat GetAngle() const;
        glm::vec3 GetAxis() const;
        void Rotate(const glm::vec4& pAngleAxis);
        void Rotate(afloat pAngle, const glm::vec3& pAxis);

        void LookAt(const glm::vec3& pLookAt, const glm::vec3& pUp = glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 GetFrontDir();
        glm::vec3 GetUpDir();
        glm::vec3 GetRightDir();

        void SetPerspective(afloat pFOV, afloat pAspectRatio, afloat pNearPlane, afloat pFarPlane);
		void SetPerspectiveFOV(afloat pFOV, afloat width, afloat height, afloat pNearPlane, afloat pFarPlane);
		void SetOrthographic(afloat pLeft, afloat pRight, afloat pTop, afloat pBottom, afloat pNearPlane, afloat pFarPlane);

        afloat GetNearPlane() const { return mNearPlane; }
        afloat GetFarPlane() const { return mFarPlane; }
        afloat GetFOV() const { assert(mCameraType == CameraType::PERSPECTIVE); return mFOV; }
        afloat GetAspectRatio() const { assert(mCameraType == CameraType::PERSPECTIVE); return mAspectRatio; }
        afloat GetLeft() const { assert(mCameraType == CameraType::ORTHOGRAPHIC); return mLeft; }
        afloat GetRight() const { assert(mCameraType == CameraType::ORTHOGRAPHIC); return mRight; }
        afloat GetTop() const { assert(mCameraType == CameraType::ORTHOGRAPHIC); return mTop; }
        afloat GetBottom() const { assert(mCameraType == CameraType::ORTHOGRAPHIC); return mBottom; }

        inline glm::mat4 GetProjectionMatrix() const { return mProjection; }
        //non const because GetTransformMatrix updates it's members if dirty
        inline glm::mat4 GetViewMatrix() { return glm::inverse(GetTransformMatrix()); }
        inline glm::mat4 GetViewProjectionMatrix() { return mProjection * glm::inverse(GetTransformMatrix()); }

    private:

        CameraType mCameraType;

        glm::vec4 mClearColor;

        //for projection matrix:
        //common
        afloat mNearPlane;
        afloat mFarPlane;
        //perspective
        afloat mFOV;
        afloat mAspectRatio;
        //orthographic
        afloat mLeft;
        afloat mRight;
        afloat mBottom;
        afloat mTop;

        glm::mat4 mProjection;
        //view matrix is the matrix from the inherited transformation
    };
}
