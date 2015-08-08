#pragma once
#include "TypeInfo.h"

class Variable
{
public:
	Variable();

	
	Variable::Variable(const Variable& rhs)
	{
		m_data = rhs.m_data;
		m_typeinfo = rhs.m_typeinfo;
	}
	Variable& Variable::operator=(const Variable& rhs)
	{
		m_data = rhs.m_data;
		m_typeinfo = rhs.m_typeinfo;

		return *this;
	}

	template<typename T>
	Variable(const T& data);

	template<typename T>
	Variable(T* data);

	Variable(void* data, const TypeInfo* type);

	template<typename T>
	T& operator=(const T& data);

	template<typename T>
	T* operator=(T* data);

	inline void* GetVoidPtr();

	inline const TypeInfo* GetType() const;


	// Cast helper exists to properly handle pointer types
	template <typename T>
	struct CastHelper
	{
		static T& Cast(void *& data)
		{
			return *(T *&)(data);
		}
	};

	template <typename T>
	struct CastHelper<T *>
	{
		static T *& Cast(void *& data)
		{
			return (T *&)data;
		}
	};

	template <typename T>
	T& GetValue(void)
	{
		ASSERT(GET_TYPE(T) == m_typeinfo);
		return CastHelper<T>::Cast(m_data);
	}

	void ToLua(lua_State* L) 
	{
		m_typeinfo->ToLua(L, *this);
	}

	void FromLua(lua_State *L, int index) 
	{
		m_typeinfo->FromLua(L, index, this);
	}

private:

	void* m_data;
	const TypeInfo* m_typeinfo;
};

Variable::Variable()
{
	m_data = nullptr;
	m_typeinfo = nullptr;
}
Variable::Variable(void* data,const TypeInfo* type)
	: m_data(data)
	, m_typeinfo(type)
	{}

template<typename T>
Variable::Variable(const T& data)
{
	m_data = (void*)&data;
	m_typeinfo = GET_TYPE(T);
}
template<typename T>
Variable::Variable(T* data)
{
	m_data = (void*)data;
	m_typeinfo = GET_TYPE(T*);
}

template<typename T>
T& Variable::operator=(const T& data)
{
	m_data = (void*)&data;
	m_typeinfo = GET_TYPE(T);
	return data;
}

template<typename T>
T* Variable::operator=(T* data)
{
	m_data = (void*)data;
	m_typeinfo = GET_TYPE(T*);
	return data;
}	

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


void GenericCopyToLua(lua_State *L, Variable& var)
{
	char* lua_mem = (char*)lua_newuserdata(L, sizeof(Variable) + var.GetType()->Size());
	Variable *reference = new (lua_mem)Variable(lua_mem + sizeof(Variable), var.GetType());
	
	memcpy(reference->GetVoidPtr(), var.GetVoidPtr(), var.GetType()->Size() );

	luaL_getmetatable(L, var.GetType()->MetaTableName());
	ASSERT(lua_type(L, -1) == LUA_TTABLE); // Ensure table exists
	lua_setmetatable(L, -2);
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
	ASSERT(lua_isuserdata(L, index));
	*var = *((Variable *)lua_touserdata(L, index));
}
