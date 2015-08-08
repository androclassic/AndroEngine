#pragma once
extern "C" {
#include "../Lua/lua.h"
#include "../Lua/lualib.h"
#include "../Lua/lauxlib.h"
}

#include"AutoList.h"
#include "Trace.h"

class Variable;

typedef void(*ToLuaFn)(lua_State *, Variable&);
typedef void(*FromLuaFn)(lua_State *, int, Variable *);

class TypeInfo : public AutoList<TypeInfo>
{
public:
	inline const char* TypeName()const { return type; }
	inline const unsigned int Size()const { return size; }
	inline const char* MetaTableName()const { return metaTable; }

	void ToLua(lua_State *L, Variable& v) const { toLuaFun(L, v); }
	void FromLua(lua_State *L, int idx, Variable *v) const { fromLuaFun(L, idx, v); }


private:
	char* type;
	unsigned int size;
	char* metaTable;

	ToLuaFn toLuaFun;
	FromLuaFn fromLuaFun;

	friend class Introspection;
};

class Introspection
{
public:
	static Introspection& GetInstance()
	{
		static Introspection instance;
		return instance;
	}

	template<typename T>
	const TypeInfo* GetTypeInfo()
	{
		TypeInfo* typeInfo = _GetTypeInfo<T>();
		return typeInfo;
	}

	template<typename T>
	void RegisterType(unsigned int size, char* name, char* metaTable , ToLuaFn toF, FromLuaFn fromF )
	{
		TypeInfo* typeinfo = _GetTypeInfo<T>();
		typeinfo->size = size;
		typeinfo->type = name;
		typeinfo->metaTable = metaTable;
		typeinfo->toLuaFun = toF;
		typeinfo->fromLuaFun = fromF;
	}

private:

	template<typename T>
	TypeInfo* _GetTypeInfo()
	{
		static TypeInfo instance;
		return &instance;
	}

	Introspection() {};
	Introspection(const Introspection& i) {}
	~Introspection() {}
};

void CharToLua(lua_State *lua, Variable& var);
void CharFromLua(lua_State *lua, int index, Variable *ref);
void CharPToLua(lua_State *lua, Variable& var);
void CharPTFromLua(lua_State *lua, int index, Variable *ref);
void IntToLua(lua_State *L, Variable& var);
void UIntToLua(lua_State *L, Variable& var);
void IntFromLua(lua_State *L, int index, Variable *ref);
void FloatToLua(lua_State *L, Variable& var);
void FloatFromLua(lua_State *L, int index, Variable *ref);
void DoubleFromLua(lua_State *L, int index, Variable *ref);
void BoolToLua(lua_State *L, Variable& var);
void BoolFromLua(lua_State *L, int index, Variable *ref);

void GenericToLua(lua_State *L, Variable& var);
void GenericCopyToLua(lua_State *L, Variable& var);
void GenericFromLua(lua_State *L, int index, Variable *var);


template<typename T>
struct ObjectRef
{
	T* object;
};

template<typename T>
void ObjRefToLua(lua_State *L, Variable& var)
{
	Variable reference = *((ObjectRef<T> *)var.GetVoidPtr())->object;
	GenericToLua(L, reference);
}


#define AS_STRING( s ) #s

#define REGISTER_TYPE(T, toLua, fromLua) Introspection::GetInstance().RegisterType<T>(sizeof(T), #T, AS_STRING(MT_##T) ,toLua, fromLua)
#define REGISTER_USER_TYPE( T ) Introspection::GetInstance().RegisterType<T>(sizeof(T), #T, AS_STRING(MT_##T), GenericToLua, GenericFromLua)

#define REGISTER_USER_TYPE_REF( T ) Introspection::GetInstance().RegisterType<ObjectRef<T>>(sizeof(ObjectRef<T>), AS_STRING(REF_##T), AS_STRING(MT_REF_##T), ObjRefToLua<T>, GenericFromLua)

#define REGISTER_USER_TYPE_POD( T ) Introspection::GetInstance().RegisterType<T>(sizeof(T), #T, AS_STRING(MT_##T), GenericCopyToLua, GenericFromLua)

#define GET_TYPE(T) Introspection::GetInstance().GetTypeInfo<T>()
#define GET_TYPE_OBJ(T) Introspection::GetInstance().GetTypeInfo<decltype(T)>()

#define REGISTER_META 	REGISTER_TYPE(bool, BoolToLua, BoolFromLua);\
						REGISTER_TYPE(char,CharToLua, CharFromLua);\
						REGISTER_TYPE(char*,CharPToLua, CharPTFromLua);\
						REGISTER_TYPE(const char*,CharPToLua, CharPTFromLua);\
						REGISTER_TYPE(int, IntToLua, IntFromLua);\
						REGISTER_TYPE(float, FloatToLua, FloatFromLua);\
						REGISTER_TYPE(double, FloatToLua, DoubleFromLua); \
						REGISTER_TYPE(unsigned char, CharToLua, CharFromLua);\
						REGISTER_TYPE(unsigned int, UIntToLua, IntFromLua);


void InitEnvironment(lua_State* L)
{
	REGISTER_META
	const TypeInfo* type = TypeInfo::Head();

	while(type != nullptr)
	{
		luaL_getmetatable(L, type->MetaTableName()); // a
		if (lua_isnil(L, 1))
		{
			lua_pop(L, 1);
			luaL_newmetatable(L, type->MetaTableName()); // a
			lua_pushstring(L, "__index"); // b
			lua_pushvalue(L, -2); // c
			lua_settable(L, -3); // a[b] = c
			lua_settop(L, 0);
		}
		else
			lua_pop(L, 1);

		type = type->Next();
	}

}
