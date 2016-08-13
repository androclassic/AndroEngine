#pragma once
#include <math.h>
#include "AndroUtils.h"


namespace andro
{
	#define DEG2RAD(x) ((x)*0.0174532925f)
	namespace math
	{
		static afloat PI = 3.14159265f;
	}

	class Vector2
	{
		public:
			Vector2(afloat xx , afloat yy ): x(xx), y(yy){};
			Vector2(): x(0.0f), y(0.0f){};
	
			afloat Lenght() const;
			void Normalize();
			Vector2 operator+(const Vector2 &rhs) const;
			Vector2 operator-(const Vector2 &rhs) const;
			afloat   operator*(const Vector2& rhs) const;
			Vector2& operator=(const Vector2 &rhs);

			afloat x;
			afloat y;
	};


	class Vector3
	{
		public:
			Vector3(afloat xx) : x(xx), y(xx), z(xx){};
			Vector3(afloat xx, afloat yy, afloat zz) : x(xx), y(yy), z(zz){};
			Vector3(const afloat v[3]) : x(v[0]), y(v[1]), z(v[2]){};
			Vector3() : x(0.0f), y(0.0f), z(0.0f){};
	
			void NormalizeInto();

			Vector3  operator+(const Vector3 &rhs) const;
			Vector3  operator-(const Vector3 &rhs) const;
			afloat	 operator*(const Vector3& rhs) const;
			Vector3  operator*(afloat scalar) const;
			Vector3& operator=(const Vector3 &rhs);
			afloat&	 operator[](unsigned int i);

			Vector3 vectorProduct(const Vector3 &vector) const;

			inline Vector3 Vector3::Multiply(const Vector3 &vector) const 	{ return Vector3(x*vector.x, y*vector.y, z*vector.z); }

			inline Vector3 Min(const Vector3 &rhs) const { return Vector3(fminf(x, rhs.x), fminf(y, rhs.y), fminf(z, rhs.z)); }
			inline Vector3 Max(const Vector3 &rhs) const { return Vector3(fmaxf(x, rhs.x), fmaxf(y, rhs.y), fmaxf(z, rhs.z)); }

			inline Vector3 Vector3::Normalise() const
			{
				Vector3 v;
				afloat lenght = Lenght();
				v.x = x / lenght;
				v.y = y / lenght;
				v.z = z / lenght;
				return v;
			}
			inline afloat Vector3::Lenght() const
			{
				return sqrt(x*x + y*y + z*z);
			}

			inline afloat Vector3::LenghtSq() const
			{
				return (x*x + y*y + z*z);
			}



			union
			{
				struct
				{
					afloat x, y, z;
				};

				afloat v[3];
			};
	};

	class Vector4
	{
		public:
			Vector4(afloat xx = 0.f, afloat yy = 0.f, afloat zz = 0.f, afloat ww = 1.f): x(xx), y(yy), z(zz), w(ww){};
			Vector4(const Vector3& rhs){ x = rhs.x, y = rhs.y; z = rhs.z; w = 1; }
			afloat x;
			afloat y;
			afloat z;
			afloat w;
	};

	class Matrix4
	{
	public:
	   Matrix4();

	   Matrix4(afloat _m11,afloat _m21,afloat _m31,afloat _m41,
			   afloat _m12,afloat _m22,afloat _m32,afloat _m42,
			   afloat _m13,afloat _m23,afloat _m33,afloat _m43,
			   afloat _m14,afloat _m24,afloat _m34,afloat _m44);
	   Matrix4 operator*( const Matrix4& mat) const;

	   friend Vector4 operator*(const Vector4& vec, const Matrix4& mat);
	   friend Vector3 operator*(const Vector3& vec, const Matrix4& mat);

	   Matrix4& operator=(const Matrix4& mat);
	   void LoadIdentity();
	   void LoadNull();

	   union{
		       
		   struct
		   { 
			   afloat  m11;afloat  m21;afloat  m31;afloat  m41;
			   afloat  m12;afloat  m22;afloat  m32;afloat  m42;
			   afloat  m13;afloat  m23;afloat  m33;afloat  m43;
			   afloat  m14;afloat  m24;afloat  m34;afloat  m44;
		   };
		   afloat matrix[4][4];
		   afloat data[16];

	   };
	};


	struct BoundingBox;



	void  RotateYaw(Matrix4&  mat,afloat angle);
	void  RotatePitch(Matrix4& mat, afloat angle);
	void  RotateRoll(Matrix4& mat, afloat angle);
	void  Translate(Matrix4& mat, afloat x, afloat y, afloat z);
	void  Scale(Matrix4& mat, afloat x, afloat y, afloat z);
	void  SetPos(Matrix4& mat, afloat x, afloat y, afloat z);
	void ComputeFrontAndRight(Vector3& front, Vector3& right, afloat pitch, afloat heading);
	afloat Distance(const Vector3& p1, const Vector3& p2);



	//TODO move
	void CreateCubeModel(const BoundingBox& aabb, Vector3 points[8], unsigned int  indices[36]);
	BoundingBox GetMinimumBoundingBox(Vector3* points, unsigned int num);



	Vector3 random_in_unit_sphere();

}