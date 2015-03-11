#pragma once

#include "MMath.h"

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
		Matrix4 m_CamMatrix;
		Matrix4 m_projection;

		Vector3 m_position;
		float m_yaw;
		float m_pitch;
		
	
 };
}


