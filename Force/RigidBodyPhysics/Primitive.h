#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include"RigidBody.h"

namespace force
{
	class Primitive
	{
		public:
			Primitive():rigidBody(NULL){}
	        Vector3 getAxis(unsigned index) const
				{
					return rigidBody->transformMatrix.getAxisVector(index);
				}

			RigidBody* rigidBody;

	};

	class Sphere : public Primitive
	{

		public:
			 Sphere(real rad)
			 {
				 radius = rad;
			 }
			real radius;
	};

	class Plane : public Primitive
	{
		public:
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
			 }
			real radius;
			real height;
	};


}

#endif