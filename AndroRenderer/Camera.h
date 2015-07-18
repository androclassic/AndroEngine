#pragma once

#include "MMath.h"

namespace andro
{
 class Camera 
 {
	public:
		Camera();

		void Set();
		
		void SetPosition(bssFloat x, bssFloat y, bssFloat z);
		void LookAt(Vector3 pos, Vector3 lookAt);
		Vector3 GetPosition() const;
		Vector3 GetOrientation() const;

		void Move(bssFloat dx, bssFloat dz);
		
		void SetYaw(bssFloat yaw); 
		void AddYaw(bssFloat yaw); 
		
		void SetPitch(bssFloat pitch);
		void AddPitch(bssFloat pitch);
	
		void setOrthoMatrix(bssFloat l, bssFloat r, bssFloat t, bssFloat b, bssFloat n, bssFloat f);
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
		bssFloat m_yaw;
		bssFloat m_pitch;
		
	
 };
}


