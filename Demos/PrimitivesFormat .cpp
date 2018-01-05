#include "PrimitivesFormat.h"
#include "../AndroUtils/Utils/AndroUtils.h"

namespace Primitives
{
	ObjectRef<force::Primitive> Sphere(float radius, float mass)
	{
		ObjectRef<force::Primitive> ret;
		ret.object =  new force::Sphere(radius);
		ret.object->rigidBody->SetMass(mass);
		return ret;
	}
	ObjectRef<force::Primitive> Box(glm::vec3 halfSize, float mass)
	{
		ObjectRef<force::Primitive> ret;
		ret.object = new force::Box(force::Vector3(halfSize.x, halfSize.y, halfSize.z));
		ret.object->rigidBody->SetMass(mass);
		return ret;
	}

	ObjectRef<force::Primitive> Cylinder(float radius, float height, float mass)
	{
		ObjectRef<force::Primitive> ret;
		ret.object = new force::Cylinder(radius, height);
		ret.object->rigidBody->SetMass(mass);
		return ret;
	}


	ObjectRef<force::Primitive> Plane(glm::vec3 normal, float offset, float mass)
	{
		ObjectRef<force::Primitive> ret;
		ret.object = new force::Plane(force::Vector3(normal.x, normal.y, normal.z), offset);
		ret.object->rigidBody->SetMass(mass);
		return ret;
	}

}
