#pragma once
#include <memory.h>

#include "../AndroUtils/Introspection/LuaState.h"
#include "../Force/core/Force.h"
#include "../../AndroSDK/glm/glm/glm.hpp"


namespace GameBindings
{
	void glmVec3FromLua(lua_State * L, int index, Variable * ref);
	void glmVec3ToLua(lua_State * L, Variable & var);

	struct PrimitiveDesc
	{
		force::PrimitiveType type;
		glm::vec3 halfSize;
		glm::vec3 normal;
		float radius;
		float height;
		float mass;
		float offset;


		static void FromLua(lua_State * L, int index, Variable * ref);
		static void ToLua(lua_State * L, Variable & var);

	};

}
