#include"Camera.h"

namespace andro
	{

	Camera::Camera()
	{
		m_yaw=0;
		m_pitch=0;


		SetPosition(0,0,0);
		RotatePitch(m_CamMatrix,m_pitch);  
		RotateYaw(m_CamMatrix,m_yaw);

	}

	void Camera::Set()
	{
		m_CamMatrix.LoadIdentity();
		RotatePitch(m_CamMatrix,m_pitch);  
		RotateYaw(m_CamMatrix,m_yaw);
		
		// Tx=POS*Right  Ty=POS*Up  Tz=POS*Look
		m_CamMatrix.Tx=m_position * m_CamMatrix.CRight;
		m_CamMatrix.Ty=m_position * m_CamMatrix.CUp;
		m_CamMatrix.Tz=m_position * m_CamMatrix.CLook;
	 
		/*m_CamMatrix->m41=m_CamMatrix->m11*m_position.x+m_CamMatrix->m21*m_position.y+m_CamMatrix->m31*m_position.z;
		m_CamMatrix->m42=m_CamMatrix->m12*m_position.x+m_CamMatrix->m22*m_position.y+m_CamMatrix->m32*m_position.z;
		m_CamMatrix->m43=m_CamMatrix->m13*m_position.x+m_CamMatrix->m23*m_position.y+m_CamMatrix->m33*m_position.z;
	  */

	}


	void Camera::SetPosition(float x, float y, float z)
	{
	  m_position.x=-x;
	  m_position.y=-y;
	  m_position.z=-z;
	  m_CamMatrix.LoadIdentity();
	  SetPos(m_CamMatrix,-x,-y,-z);
	}

	void Camera::LookAt(Vector3 pos, Vector3 lookAt)
	{

		Vector3 up = Vector3(0,1,0);
		Vector3 zaxis = lookAt - pos;
		zaxis.Normalize();

		Vector3 xaxis = up.vectorProduct(zaxis);
		xaxis.Normalize();

		Vector3 yaxis = xaxis.vectorProduct(zaxis);

		SetPosition(pos.x,pos.y,pos.z);

		m_CamMatrix.CRight = xaxis *-1;
		m_CamMatrix.CUp = yaxis*-1;
		m_CamMatrix.CLook = zaxis*-1;


		m_CamMatrix.Tx=m_position * m_CamMatrix.CRight;
		m_CamMatrix.Ty=m_position * m_CamMatrix.CUp;
		m_CamMatrix.Tz=m_position * m_CamMatrix.CLook;


	}

	Vector3 Camera::GetPosition() const
	{
		return m_position * -1.0f;
	}

	Vector3 Camera::GetOrientation() const
	{
		Vector3 o =  m_CamMatrix.CLook;
		o.Normalize();
		return o * -1;
	}

	void Camera::SetPitch(float pitch)
	{
	   m_pitch=pitch;
	   m_pitch = fmod(m_pitch, 360);


		
	}
	void Camera::AddPitch(float pitch)
	{
	   m_pitch+=pitch;
	   m_pitch = fmod(m_pitch, 360);

	  
	}
	void Camera::SetYaw(float yaw)
	{
		m_yaw=yaw;
		m_yaw = fmod(m_yaw, 360);
	}
	void Camera::AddYaw(float yaw)
	{
		m_yaw+=yaw;
		m_yaw = fmod(m_yaw, 360);

	}
	void Camera::Move(float dx, float dz)
	{
		
		if(dz)
		{
  		 m_CamMatrix.CLook.Normalize();
		 m_position.x -= m_CamMatrix.m13 * dz;
		 m_position.y -= m_CamMatrix.m23 * dz;
		 m_position.z -= m_CamMatrix.m33 * dz;
		

		}
		else if(dx)
		{
			
  		 m_CamMatrix.CRight.Normalize();
		 m_position.x -= m_CamMatrix.m11*dx;
		 m_position.y -= m_CamMatrix.m21*dx;
		 m_position.z -= m_CamMatrix.m31*dx;
		}

	}

	void  Camera::setOrthoMatrix(float l, float r, float t, float b, float n, float f)
	{
		Matrix4& mat = m_projection;


		mat.matrix[0][0] = 2 / (r - l);
		mat.matrix[1][1] = 2 / (t - b);
		mat.matrix[2][2] = -1 / (f - n);

		mat.matrix[3][0] = -(r + l) / (r - l);
		mat.matrix[3][1] = -(t + b) / (t - b);
		mat.matrix[3][2] = -n / (f - n);
		mat.matrix[3][3] = 1;
	}

	void  Camera::setPerspectiveMatrix(double fovy, double aspect, double zNear, double zFar)
	{
		Matrix4& mat = m_projection;
		double tanHalfFovy = tan(DEG2RAD(fovy) / 2);

		mat.LoadIdentity();
		mat.matrix[0][0] = 1 / (aspect * tanHalfFovy);
		mat.matrix[1][1] = 1 / (tanHalfFovy);
		mat.matrix[2][2] = -(zFar + zNear) / (zFar - zNear);
		mat.matrix[3][2] = -1;
		mat.matrix[2][3] = -(2 * zFar * zNear) / (zFar - zNear);
	}

}
