#pragma once
#include <math.h>
#include "../AndroUtils/AndroUtils.h"


namespace andro
{
	#define DEG2RAD(x) ((x)*0.0174532925f)

	class Vector2
	{
		public:
			Vector2(float xx , float yy ): x(xx), y(yy){};
			Vector2(): x(0.0f), y(0.0f){};
	
			float Lenght() const;
			void Normalize();
			Vector2 operator+(const Vector2 &rhs) const;
			Vector2 operator-(const Vector2 &rhs) const;
			float   operator*(const Vector2& rhs) const;
			Vector2& operator=(const Vector2 &rhs);

			float x;
			float y;
	};


	class Vector3
	{
		public:
			Vector3(float xx , float yy , float zz ): x(xx), y(yy), z(zz){};
			Vector3(const float v[3]) : x(v[0]), y(v[1]), z(v[2]){};
			Vector3() : x(0.0f), y(0.0f), z(0.0f){};
	
			float Lenght() const;
			void Normalize();
			Vector3  operator+(const Vector3 &rhs) const;
			Vector3  operator-(const Vector3 &rhs) const;
			float	 operator*(const Vector3& rhs) const;
			Vector3  operator*(float scalar) const;
			Vector3& operator=(const Vector3 &rhs);
			Vector3 vectorProduct(const Vector3 &vector) const;
       
			float x;
			float y;
			float z;
	};

	class Vector4
	{
		public:
			Vector4(float xx = 0.f, float yy = 0.f, float zz = 0.f, float ww = 1.f): x(xx), y(yy), z(zz), w(ww){};
			Vector4(const Vector3& rhs){ x = rhs.x, y = rhs.y; z = rhs.z; w = 1; }
			float x;
			float y;
			float z;
			float w;
	};

	class Matrix4
	{
	public:
	   Matrix4();

	   Matrix4(float _m11,float _m21,float _m31,float _m41,
			   float _m12,float _m22,float _m32,float _m42,
			   float _m13,float _m23,float _m33,float _m43,
			   float _m14,float _m24,float _m34,float _m44);
	   Matrix4 operator*( const Matrix4& mat);

	   friend Vector4 operator*(const Vector4& vec, const Matrix4& mat);
	   friend Vector3 operator*(const Vector3& vec, const Matrix4& mat);

	   Matrix4& operator=(const Matrix4& mat);
	   void LoadIdentity();
	   void LoadNull();

	   union{
		       
		   struct
		   { 
			   float  m11;float  m21;float  m31;float  m41;
			   float  m12;float  m22;float  m32;float  m42;
			   float  m13;float  m23;float  m33;float  m43;
			   float  m14;float  m24;float  m34;float  m44;
		   };
		   float matrix[4][4];
		   float data[16];

	   };
	};

	void  RotateYaw(Matrix4&  mat,float angle);
	void  RotatePitch(Matrix4& mat, float angle);
	void  RotateRoll(Matrix4& mat, float angle);
	void  Translate(Matrix4& mat, float x, float y, float z);
	void  Scale(Matrix4& mat, float x, float y, float z);
	void  SetPos(Matrix4& mat, float x, float y, float z);
	void ComputeFrontAndRight(Vector3& front, Vector3& right, float pitch, float heading);
	float Distance(const Vector3& p1, const Vector3& p2);
}