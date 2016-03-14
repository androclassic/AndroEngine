#include "Variable.h"

Variable::Variable()
{
	m_data = nullptr;
	m_typeinfo = nullptr;
}
Variable::Variable(void* data,const TypeInfo* type)
	: m_data(data)
	, m_typeinfo(type)
	{}

void* Variable::GetVoidPtr()
{
	return m_data;
}
const TypeInfo* Variable::GetType() const
{
	return m_typeinfo; 
}



//---------------------------------------------------------------------------------
// lua type function get/push
//---------------------------------------------------------------------------------

void CharPToLua(lua_State *lua, Variable& var)
{
	lua_pushstring(lua, var.GetValue<char *>());
}

void CharPTFromLua(lua_State *lua, int index, Variable *ref)
{
	ASSERT(lua_isstring(lua, index));
	const char *temp = lua_tostring(lua, index);
	*ref = temp;
}

void IntToLua(lua_State *L, Variable& var)
{
	lua_pushinteger(L, var.GetValue<int>());
}

void UIntToLua(lua_State *L, Variable& var)
{
	lua_pushinteger(L, var.GetValue<unsigned>());
}

void IntFromLua(lua_State *L, int index, Variable *ref)
{
	ASSERT(lua_isnumber(L, index)); // Type mismatch! Passed wrong argument type in Lua call to C++ function?
	int temp = lua_tointeger(L, index);
	memcpy(ref->GetVoidPtr(), &temp, sizeof(temp));
}

void FloatToLua(lua_State *L, Variable& var)
{
	lua_pushnumber(L, var.GetValue<float>());
}

void FloatFromLua(lua_State *L, int index, Variable *ref)
{
	ASSERT(lua_isnumber(L, index)); // Type mismatch! Passed wrong argument type in Lua call to C++ function?
	float temp = (float)lua_tonumber(L, index);
	*(float*)ref->GetVoidPtr() = temp;
}

void DoubleFromLua(lua_State *L, int index, Variable *ref)
{
	ASSERT(lua_isnumber(L, index)); // Type mismatch! Passed wrong argument type in Lua call to C++ function?
	float temp = (float)lua_tonumber(L, index);
	*(double*)ref->GetVoidPtr() = temp;
}


void BoolToLua(lua_State *L, Variable& var)
{
	lua_pushboolean(L, var.GetValue<bool>());
}

void BoolFromLua(lua_State *L, int index, Variable *ref)
{
	ASSERT(lua_isboolean(L, index)); // Type mismatch! Passed wrong argument type in Lua call to C++ function?
	bool temp = (bool)lua_toboolean(L, index);
	memcpy(ref->GetVoidPtr(), &temp, sizeof(temp));
}

void CharToLua(lua_State *L, Variable& var)
{
	lua_pushinteger(L, var.GetValue<char>());
}

void CharFromLua(lua_State *L, int index, Variable *ref)
{
	ASSERT(lua_isstring(L, index)); // Type mismatch! Passed wrong argument type in Lua call to C++ function?
	const char*  temp = lua_tostring(L, index);
	memcpy(ref->GetVoidPtr(), temp, sizeof(char));
}

void GenericToLua(lua_State *L, Variable& var)
{
	Variable *reference = (Variable *)lua_newuserdata(L, sizeof(Variable));
	new (reference)Variable(var);
	luaL_getmetatable(L, var.GetType()->MetaTableName());
	ASSERT(lua_type(L, -1) == LUA_TTABLE); // Ensure table exists
	lua_setmetatable(L, -2);
}


void GenericFromLua(lua_State *L, int index, Variable *var)
{
	if(lua_isuserdata(L, index))
	{
		*var = *((Variable *)lua_touserdata(L, index));
		return;
	}
	
	if (lua_istable(L, index))
	{
		lua_getfield(L, 1, "__userdata");
		if (lua_isuserdata(L, -1))
		{
			*var = *((Variable *)lua_touserdata(L, -1));
			lua_pop(L, 1);
			return;
		}
	}
	ASSERT(FALSE); //not userdata or table with it
}
