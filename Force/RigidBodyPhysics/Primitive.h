#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include"RigidBody.h"

namespace force
{
	enum PrimitiveType
	{
		BOX,
		SPHERE,
		PLANE,
		CYLINDER,
		UNKNOWN
	};

	class Primitive
	{
		public:
			Primitive() : rigidBody(&data), primitiveType(UNKNOWN) {}

	        Vector3 getAxis(unsigned index) const
				{
					return rigidBody->transformMatrix.getAxisVector(index);
				}

			RigidBody* rigidBody;
			PrimitiveType primitiveType;

			private:
				RigidBody data; // ugly but avoids code changes
	};

	class Sphere : public Primitive
	{

		public:
			 Sphere(real rad)
			 {
				 radius = rad;
				 primitiveType = PrimitiveType::SPHERE;
			 }
			real radius;
	};

	class Plane : public Primitive
	{
		public:
			Plane(Vector3 norm, real off)
			{
				primitiveType = PrimitiveType::PLANE;
				normal = norm;
				offset = off;
			}
			Vector3 normal;
			real offset;
	};
	class Box : public Primitive
	{
		public:
			Box(Vector3 hSize)
			{
             halfSize=hSize;  
			 Vector3 ver[8] =
				{
				Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
				Vector3(-halfSize.x, -halfSize.y, +halfSize.z),
				Vector3(-halfSize.x, +halfSize.y, -halfSize.z),
				Vector3(-halfSize.x, +halfSize.y, +halfSize.z),
				Vector3(+halfSize.x, -halfSize.y, -halfSize.z),
				Vector3(+halfSize.x, -halfSize.y, +halfSize.z),
				Vector3(+halfSize.x, +halfSize.y, -halfSize.z),
				Vector3(+halfSize.x, +halfSize.y, +halfSize.z)
				};
			 for(int i=0;i<8;i++)
					vertices[i] = ver[i];

			 primitiveType = PrimitiveType::BOX;
			}
		Vector3 halfSize;
		Vector3 vertices[8];
	};

	class Cylinder : public Primitive
	{

		public:
			 Cylinder(real rad, real height)
			 {
				 radius = rad;
				 this->height = height;
				 primitiveType = PrimitiveType::CYLINDER;
			 }
			real radius;
			real height;
	};


}

#endif