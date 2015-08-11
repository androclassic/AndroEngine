#pragma once

#include "../AndroUtils/MMath.h"

namespace andro
{
 class Camera 
 {
	public:
		Camera();

		void Set();
		
		void SetPosition(float x, float y, float z);
		void LookAt(Vector3 pos, Vector3 lookAt);
		Vector3 GetPosition() const;
		Vector3 GetOrientation() const;

		void Move(float dx, float dz);
		
		void SetYaw(float yaw); 
		void AddYaw(float yaw); 
		
		void SetPitch(float pitch);
		void AddPitch(float pitch);
	
		void setOrthoMatrix(float l, float r, float t, float b, float n, float f);
		void setPerspectiveMatrix(double fovy, double aspect, double zNear, double zFar);


		const Matrix4& GetTransform() const { return  m_CamMatrix; }
		const Matrix4& GetProjection()const { return  m_projection; }

	private:
	
		void SetRight(const Vector3& v)		{ m_CamMatrix.m11 = v.x; m_CamMatrix.m21 = v.y; m_CamMatrix.m31 = v.z; }
		void SetUp(const Vector3& v)		{ m_CamMatrix.m12 = v.x; m_CamMatrix.m22 = v.y; m_CamMatrix.m32 = v.z; }
		void SetLook(const Vector3& v)		{ m_CamMatrix.m13 = v.x; m_CamMatrix.m23 = v.y; m_CamMatrix.m33 = v.z; }

		Vector3 GetRight() const	{ return Vector3(m_CamMatrix.m11, m_CamMatrix.m21, m_CamMatrix.m31); }
		Vector3 GetUp()	const		{ return Vector3(m_CamMatrix.m12, m_CamMatrix.m22, m_CamMatrix.m32); }
		Vector3 GetLook() const		{ return Vector3(m_CamMatrix.m13, m_CamMatrix.m23, m_CamMatrix.m33); }

		Matrix4 m_CamMatrix;
		Matrix4 m_projection;

		Vector3 m_position;
		float m_yaw;
		float m_pitch;
		
	
 };
}


