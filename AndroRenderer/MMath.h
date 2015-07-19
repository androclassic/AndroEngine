#pragma once
#include <math.h>
#include "../AndroUtils/AndroUtils.h"


namespace andro
{
	#define DEG2RAD(x) ((x)*0.0174532925f)

	class Vector2
	{
		public:
			Vector2(bssFloat xx , bssFloat yy ): x(xx), y(yy){};
			Vector2(): x(0.0f), y(0.0f){};
	
			bssFloat Lenght() const;
			void Normalize();
			Vector2 operator+(const Vector2 &rhs) const;
			Vector2 operator-(const Vector2 &rhs) const;
			bssFloat   operator*(const Vector2& rhs) const;
			Vector2& operator=(const Vector2 &rhs);

			bssFloat x;
			bssFloat y;
	};


	class Vector3
	{
		public:
			Vector3(bssFloat xx , bssFloat yy , bssFloat zz ): x(xx), y(yy), z(zz){};
			Vector3(const bssFloat v[3]) : x(v[0]), y(v[1]), z(v[2]){};
			Vector3() : x(0.0f), y(0.0f), z(0.0f){};
	
			bssFloat Lenght() const;
			void Normalize();
			Vector3  operator+(const Vector3 &rhs) const;
			Vector3  operator-(const Vector3 &rhs) const;
			bssFloat	 operator*(const Vector3& rhs) const;
			Vector3  operator*(bssFloat scalar) const;
			Vector3& operator=(const Vector3 &rhs);
			Vector3 vectorProduct(const Vector3 &vector) const;
       
			bssFloat x;
			bssFloat y;
			bssFloat z;
	};

	class Vector4
	{
		public:
			Vector4(bssFloat xx = 0.f, bssFloat yy = 0.f, bssFloat zz = 0.f, bssFloat ww = 1.f): x(xx), y(yy), z(zz), w(ww){};
			Vector4(const Vector3& rhs){ x = rhs.x, y = rhs.y; z = rhs.z; w = 1; }
			bssFloat x;
			bssFloat y;
			bssFloat z;
			bssFloat w;
	};

	class Matrix4
	{
	public:
	   Matrix4();

	   Matrix4(bssFloat _m11,bssFloat _m21,bssFloat _m31,bssFloat _m41,
			   bssFloat _m12,bssFloat _m22,bssFloat _m32,bssFloat _m42,
			   bssFloat _m13,bssFloat _m23,bssFloat _m33,bssFloat _m43,
			   bssFloat _m14,bssFloat _m24,bssFloat _m34,bssFloat _m44);
	   Matrix4 operator*( const Matrix4& mat);

	   friend Vector4 operator*(const Vector4& vec, const Matrix4& mat);
	   friend Vector3 operator*(const Vector3& vec, const Matrix4& mat);

	   Matrix4& operator=(const Matrix4& mat);
	   void LoadIdentity();
	   void LoadNull();

	   union{
		       
		   struct
		   { 
			   bssFloat  m11;bssFloat  m21;bssFloat  m31;bssFloat  m41;
			   bssFloat  m12;bssFloat  m22;bssFloat  m32;bssFloat  m42;
			   bssFloat  m13;bssFloat  m23;bssFloat  m33;bssFloat  m43;
			   bssFloat  m14;bssFloat  m24;bssFloat  m34;bssFloat  m44;
		   };
		   bssFloat matrix[4][4];
		   bssFloat data[16];

	   };
	};

	void  RotateYaw(Matrix4&  mat,bssFloat angle);
	void  RotatePitch(Matrix4& mat, bssFloat angle);
	void  RotateRoll(Matrix4& mat, bssFloat angle);
	void  Translate(Matrix4& mat, bssFloat x, bssFloat y, bssFloat z);
	void  Scale(Matrix4& mat, bssFloat x, bssFloat y, bssFloat z);
	void  SetPos(Matrix4& mat, bssFloat x, bssFloat y, bssFloat z);
	void ComputeFrontAndRight(Vector3& front, Vector3& right, bssFloat pitch, bssFloat heading);
	bssFloat Distance(const Vector3& p1, const Vector3& p2);
}