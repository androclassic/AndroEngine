#include "MMath.h"
#include <math.h>
#include "Shapes.h"


namespace andro
{
	void ComputeFrontAndRight(Vector3& front, Vector3& right, afloat pitch, afloat heading)
	{
		afloat sinP = sinf(DEG2RAD(pitch));
		afloat cosP = cosf(DEG2RAD(pitch));

		afloat sinH = sinf(DEG2RAD(heading));
		afloat cosH = cosf(DEG2RAD(heading));

		right.x = cosP;
		right.y = 0;
		right.z = sinP;

		front.x = -sinP * cosH;
		front.y = sinH;
		front.z = cosP * cosH;
	}

#if defined(_USE_CUDA) && !defined(ANDRO_UTILS) ||  !defined(_USE_CUDA) 
	DEVICE_HOST afloat Vector2::operator* (const Vector2& rhs) const
	{
		return x*rhs.x + y*rhs.y;
	}



	DEVICE_HOST Vector2& Vector2::operator=(const Vector2& rhs)
	{
		x=rhs.x;
		y=rhs.y;

		return *this;
	}
	DEVICE_HOST void Vector2::Normalize()
	{
		afloat lenght=Lenght();
		x=x/lenght;
		y=y/lenght;
	}

	DEVICE_HOST Vector2 Vector2::operator+(const Vector2& rhs)const
	{
		return Vector2(x+rhs.x,y+rhs.y);
	}
	DEVICE_HOST Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return Vector2(x-rhs.x,y-rhs.y);
	}


	DEVICE_HOST afloat Vector2::Lenght() const
	{
		return sqrt(x*x + y*y);
	}

	DEVICE_HOST afloat Vector3::operator*(const Vector3& rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	DEVICE_HOST Vector3 Vector3::vectorProduct(const Vector3 &vector) const
    {
        return Vector3(y*vector.z-z*vector.y,
                        z*vector.x-x*vector.z,
                        x*vector.y-y*vector.x);
    }

	DEVICE_HOST Vector3& Vector3::operator=(const Vector3& rhs)
	{
		x=rhs.x;
		y=rhs.y;
		z=rhs.z;

		return *this;
	}
	DEVICE_HOST afloat & Vector3::operator[](unsigned int i)
	{
		ASSERT(i < 3);
		return this->v[i];
	}
	DEVICE_HOST void Vector3::NormalizeInto()
	{
		afloat lenght=Lenght();
		x=x/lenght;
		y=y/lenght;
		z=z/lenght;
	}


	DEVICE_HOST Vector3 Vector3::operator+(const Vector3& rhs) const
	{
		return Vector3(x+rhs.x,y+rhs.y,z+rhs.z);
	}
	DEVICE_HOST Vector3 Vector3::operator-(const Vector3& rhs) const
	{
		return Vector3(x-rhs.x,y-rhs.y,z-rhs.z);
	}

	DEVICE_HOST Vector3 Vector3::operator*(afloat scalar) const
	{
		return Vector3(scalar * x,scalar * y,scalar * z);
	}


	//----------------------------------------------------------------------------------


	DEVICE Vector3 random_in_unit_sphere()
	{
		Vector3 random_vector;
		do
		{

			random_vector = Vector3(random_float(2.0f) - 1.0f, random_float(2.0f) - 1.0f, random_float(2.0f) - 1.0f);
		} while (random_vector.Lenght() > 1.0f);

		return random_vector;
	}

	//----------------------------------------------------------------------------------
#endif //defined(_USE_CUDA) && !defined(ANDRO_UTILS) ||  !defined(_USE_CUDA) 

	afloat Distance(const Vector3& p1, const  Vector3& p2)
	{
	  return	sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2));
	}




	Matrix4::Matrix4()
	{
		LoadIdentity();

	}


	Matrix4::Matrix4(afloat _m11, afloat _m12, afloat _m13, afloat _m14,
					 afloat _m21, afloat _m22, afloat _m23, afloat _m24,
					 afloat _m31, afloat _m32, afloat _m33, afloat _m34,
					 afloat _m41, afloat _m42, afloat _m43, afloat _m44)
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






	void RotateYaw(Matrix4 &mat, afloat angle)
	{
			angle=DEG2RAD(angle);//convertion

	  Matrix4 rotate(cos(angle),  0,  sin(angle),    0,
						0      ,  1,     0      ,    0,
					-sin(angle),  0,  cos(angle),    0,
						0      ,  0,       0    ,    1);
	 
	 Matrix4 temp(mat); 
	mat =(temp*rotate);
	  
	}
	void  RotatePitch(Matrix4& mat, afloat angle)
	{
		angle=DEG2RAD(angle);//convertion

	  Matrix4 rotate(   1 ,  0        ,     0      ,    0,
						0 , cos(angle), -sin(angle),    0,
						0 , sin(angle),  cos(angle),    0,
						0 ,    0      ,       0    ,    1);
	 
	  Matrix4 temp(mat); 
	mat = (temp*rotate);
	 

	}
	void RotateRoll(Matrix4& mat, afloat angle)
	{
			angle=DEG2RAD(angle);//convertion

	 Matrix4 rotate(cos(angle),-sin(angle),     0,    0,
					 sin(angle), cos(angle),     0,    0,
						0      ,     0     ,     1,    0,
						0      ,     0     ,     0,    1);
	 Matrix4 temp(mat); 
	mat = (temp*rotate);
	}

	void  Translate(Matrix4& mat, afloat x, afloat y, afloat z)
	{
		mat.m41=x;
		mat.m42=y;
		mat.m43=z;
	}

	void  Scale(Matrix4& mat, afloat x, afloat y, afloat z)
	{
		Matrix4 scale;
		scale.LoadIdentity();

		scale.m11 = x;
		scale.m22 = y;
		scale.m33 = z;

	 Matrix4 temp(mat); 
	 mat = (temp * scale);

	}

	void SetPos(Matrix4& mat, afloat x, afloat y, afloat z)
	{
		mat.m41=x;
		mat.m42=y;
		mat.m43=z;
	}

}


