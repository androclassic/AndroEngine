#pragma once
#include <math.h>
#include "AndroUtils.h"


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


			union
			{
				struct
				{
					float x, y, z;
				};

				float v[3];
			};
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
	   Matrix4 operator*( const Matrix4& mat) const;

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


	struct Triangle
	{
		Vector3 P1, P2, P3;
	};

	struct BoundingBox
	{
		BoundingBox();
		inline void SetExtents(Vector3& pMin, Vector3&  pMax) { min = pMin; max = pMax; }
		Vector3 GetHalfSize() const;
		Vector3 GetCenter() const;

		Vector3		min;
		Vector3		max;
	};



	void  RotateYaw(Matrix4&  mat,float angle);
	void  RotatePitch(Matrix4& mat, float angle);
	void  RotateRoll(Matrix4& mat, float angle);
	void  Translate(Matrix4& mat, float x, float y, float z);
	void  Scale(Matrix4& mat, float x, float y, float z);
	void  SetPos(Matrix4& mat, float x, float y, float z);
	void ComputeFrontAndRight(Vector3& front, Vector3& right, float pitch, float heading);
	float Distance(const Vector3& p1, const Vector3& p2);



	//intersections
	bool  Box_Plane_Overlap(const BoundingBox& box, Vector3 planeNormal, float distance);
	bool  TriangleBoxOverlap(const BoundingBox& box, Triangle& triangle);


	//TODO move
	void CreateCubeModel(const BoundingBox& aabb, Vector3 points[8], unsigned int  indices[36]);
	BoundingBox GetMinimumBoundingBox(Vector3* points, unsigned int num);



	Vector3 random_in_unit_sphere();

}