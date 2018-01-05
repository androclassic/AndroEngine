#include "GameBindings.h"
#include "../AndroUtils/Utils/AndroUtils.h"

namespace GameBindings
{
	void glmVec3FromLua(lua_State * L, int index, Variable * ref)
	{
		ASSERT(lua_istable(L, index));

		glm::vec3 *ref_vec = new ((glm::vec3*)ref->GetVoidPtr())glm::vec3();

		lua_getfield(L, index, "x");
		ref_vec->x = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "y");
		ref_vec->y = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "z");
		ref_vec->z = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	void glmVec3ToLua(lua_State * L, Variable & var)
	{
		glm::vec3 vec = var.GetValue<glm::vec3 >();

		lua_newtable(L); //{*table is now in - 1 * }
		lua_pushnumber(L, vec.x); //{*table is now in - 2 * }
		lua_setfield(L, -2, "x");
		lua_pushnumber(L, vec.y); //{*table is now in - 2 * }
		lua_setfield(L, -2, "y");
		lua_pushnumber(L, vec.z); //{*table is now in - 2 * }
		lua_setfield(L, -2, "z");
	}

	
	void PrimitiveDesc::FromLua(lua_State * L, int index, Variable * ref)
	{
		ASSERT(lua_istable(L, index));

		PrimitiveDesc *ref_prim = new ((PrimitiveDesc*)ref->GetVoidPtr())PrimitiveDesc();


		lua_getfield(L, index, "primitiveType");
		lua_getfield(L, index, "halfSize");
		lua_getfield(L, index, "radius");
		lua_getfield(L, index, "normal");
		lua_getfield(L, index, "offset");
		lua_getfield(L, index, "height");
		lua_getfield(L, index, "mass");

		glm::vec3 temp;
		Variable tempVar = temp;
		ref_prim->type = (force::PrimitiveType)lua_tointeger(L, -7);

		glmVec3FromLua(L, -6, &tempVar);
		ref_prim->halfSize = tempVar.GetValue<glm::vec3>();

		ref_prim->radius = lua_tonumber(L, -5);
		
		glmVec3FromLua(L, -4, &tempVar);
		ref_prim->normal = tempVar.GetValue<glm::vec3>();

		ref_prim->offset = lua_tonumber(L, -3);
		ref_prim->height = lua_tonumber(L, -2);
		ref_prim->mass = lua_tonumber(L, -1);
	}

	void PrimitiveDesc::ToLua(lua_State * L, Variable & var)
	{
		ASSERT(FALSE);
	}

	
}
