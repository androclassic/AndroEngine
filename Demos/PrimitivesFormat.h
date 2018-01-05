#pragma once
#include <memory.h>

#include "../AndroUtils/Introspection/LuaState.h"
#include "../Force/core/Force.h"
#include "../../AndroSDK/glm/glm/glm.hpp"


namespace Primitives
{
	ObjectRef<force::Primitive> Sphere(float radius, float mass);
	ObjectRef<force::Primitive> Box(glm::vec3 halfSize, float mass);
	ObjectRef<force::Primitive> Cylinder(float radius, float height, float mass);
	ObjectRef<force::Primitive> Plane(glm::vec3 normal, float offset, float mass);

	
}