#include "MMath.h"
#include <math.h>
namespace andro
{
	void ComputeFrontAndRight(Vector3& front, Vector3& right, float pitch, float heading)
	{
		float sinP = sinf(DEG2RAD(pitch));
		float cosP = cosf(DEG2RAD(pitch));

		float sinH = sinf(DEG2RAD(heading));
		float cosH = cosf(DEG2RAD(heading));

		right.x = cosP;
		right.y = 0;
		right.z = sinP;

		front.x = -sinP * cosH;
		front.y = sinH;
		front.z = cosP * cosH;
	}


	float Vector2::operator* (const Vector2& rhs) const
	{
		return x*rhs.x + y*rhs.y;
	}



	Vector2& Vector2::operator=(const Vector2& rhs) 
	{
		x=rhs.x;
		y=rhs.y;

		return *this;
	}
	void Vector2::Normalize()
	{
		float lenght=Lenght();
		x=x/lenght;
		y=y/lenght;
	}

	Vector2 Vector2::operator+(const Vector2& rhs)const
	{
		return Vector2(x+rhs.x,y+rhs.y);
	}
	Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return Vector2(x-rhs.x,y-rhs.y);
	}


	float Vector2::Lenght() const
	{
		return sqrt(x*x + y*y);
	}

	float Vector3::operator *(const Vector3& rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	Vector3 Vector3::vectorProduct(const Vector3 &vector) const
    {
        return Vector3(y*vector.z-z*vector.y,
                        z*vector.x-x*vector.z,
                        x*vector.y-y*vector.x);
    }

	Vector3& Vector3::operator=(const Vector3& rhs)
	{
		x=rhs.x;
		y=rhs.y;
		z=rhs.z;

		return *this;
	}
	void Vector3::Normalize()
	{
		float lenght=Lenght();
		x=x/lenght;
		y=y/lenght;
		z=z/lenght;
	}

	Vector3 Vector3::operator+(const Vector3& rhs) const
	{
		return Vector3(x+rhs.x,y+rhs.y,z+rhs.z);
	}
	Vector3 Vector3::operator-(const Vector3& rhs) const
	{
		return Vector3(x-rhs.x,y-rhs.y,z-rhs.z);
	}

	Vector3 Vector3::operator*(float scalar) const
	{
		return Vector3(scalar * x,scalar * y,scalar * z);
	}


	float Vector3::Lenght() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Distance(const Vector3& p1, const  Vector3& p2)
	{
	  return	sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2));
	}
	bool TriangleBoxOverlap(Vector3 center, Vector3 halfSize, Triangle & triangle)
	{
		Vector3 dif1 = triangle.P1 - center;
		Vector3 dif2 = triangle.P2 - center;
		Vector3 dif3 = triangle.P3 - center;

		float len = halfSize.Lenght();

		if (dif1.Lenght() < len )
			return true;

		return false;
	}
	Matrix4::Matrix4()
	{
		LoadIdentity();

	}


	Matrix4::Matrix4(float _m11, float _m12, float _m13, float _m14,
					 float _m21, float _m22, float _m23, float _m24,
					 float _m31, float _m32, float _m33, float _m34,
					 float _m41, float _m42, float _m43, float _m44)
	{
		
	   m11=_m11;m12=_m12;m13=_m13;m14=_m14;
	   m21=_m21;m22=_m22;m23=_m23;m24=_m24;
	   m31=_m31;m32=_m32;m33=_m33;m34=_m34;
	   m41=_m41;m42=_m42;m43=_m43;m44=_m44;

	}
	void Matrix4::LoadIdentity()
	{
		for(unsigned  int i=0;i<4;i++)
			
		{
			for(unsigned int j=0;j<4;j++)
			{
				if(j==i)
					matrix[j][i]=1;
				else
				matrix[j][i]=0;
			}
		}
	}
	void Matrix4::LoadNull()
	{
		for(unsigned  int i=0;i<4;i++)
			
		{
			for(unsigned int j=0;j<4;j++)
			{
				matrix[j][i]=0;
			}
		}
	}
	Matrix4 Matrix4::operator *(const Matrix4& mat) const
	{
		Matrix4 rhs;
		rhs.LoadNull();

		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				for(int k=0;k<4;k++)
				   rhs.matrix[i][j]+=matrix[i][k]*mat.matrix[k][j];
		return rhs;

	}
	Vector4 operator*(const Vector4& vec, const Matrix4& mat)
	{
	   Vector4 rhs;

	   rhs.x=vec.x*mat.m11+vec.y*mat.m21+vec.z*mat.m31+vec.w*mat.m41;
	   rhs.y=vec.x*mat.m12+vec.y*mat.m22+vec.z*mat.m32+vec.w*mat.m42;
	   rhs.z=vec.x*mat.m13+vec.y*mat.m23+vec.z*mat.m33+vec.w*mat.m43;
	   rhs.w=vec.x*mat.m14+vec.y*mat.m24+vec.z*mat.m34+vec.w*mat.m44;
	return rhs;
	}
	Vector3 operator*(const Vector3& vec, const Matrix4& mat)
	{
	   Vector3 rhs;

	   rhs.x=vec.x*mat.m11+vec.y*mat.m21+vec.z*mat.m31+ mat.m41;
	   rhs.y=vec.x*mat.m12+vec.y*mat.m22+vec.z*mat.m32+ mat.m42;
	   rhs.z=vec.x*mat.m13+vec.y*mat.m23+vec.z*mat.m33+ mat.m43;

	   return rhs;
	}

	Matrix4& Matrix4::operator=(const Matrix4& mat)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				matrix[i][j]=mat.matrix[i][j];

		return *this;
	}

	void RotateYaw(Matrix4 &mat, float angle)
	{
			angle=DEG2RAD(angle);//convertion

	  Matrix4 rotate(cos(angle),  0,  sin(angle),    0,
						0      ,  1,     0      ,    0,
					-sin(angle),  0,  cos(angle),    0,
						0      ,  0,       0    ,    1);
	 
	 Matrix4 temp(mat); 
	mat =(temp*rotate);
	  
	}
	void  RotatePitch(Matrix4& mat, float angle)
	{
		angle=DEG2RAD(angle);//convertion

	  Matrix4 rotate(   1 ,  0        ,     0      ,    0,
						0 , cos(angle), -sin(angle),    0,
						0 , sin(angle),  cos(angle),    0,
						0 ,    0      ,       0    ,    1);
	 
	  Matrix4 temp(mat); 
	mat = (temp*rotate);
	 

	}
	void RotateRoll(Matrix4& mat, float angle)
	{
			angle=DEG2RAD(angle);//convertion

	 Matrix4 rotate(cos(angle),-sin(angle),     0,    0,
					 sin(angle), cos(angle),     0,    0,
						0      ,     0     ,     1,    0,
						0      ,     0     ,     0,    1);
	 Matrix4 temp(mat); 
	mat = (temp*rotate);
	}

	void  Translate(Matrix4& mat, float x, float y, float z)
	{
		mat.m41=x;
		mat.m42=y;
		mat.m43=z;
	}

	void  Scale(Matrix4& mat, float x, float y, float z)
	{
		Matrix4 scale;
		scale.LoadIdentity();

		scale.m11 = x;
		scale.m22 = y;
		scale.m33 = z;

	 Matrix4 temp(mat); 
	 mat = (temp * scale);

	}

	void SetPos(Matrix4& mat, float x, float y, float z)
	{
		mat.m41=x;
		mat.m42=y;
		mat.m43=z;
	}
}