#include "LuaBinding.h"


// Generic function call from Lua
int GenericFunc(lua_State *L)
{


	// Retrieve the generic functor from a Lua upvalue
	Function *fn = (Function *)lua_touserdata(L, lua_upvalueindex(1));

	ASSERT(fn != nullptr); // Make sure the upvalue actually exists

	// Retrieve the argument count from Lua
	unsigned int argCount = lua_gettop(L);

	ASSERT(fn->GetSigneture()->ArgCount() <= argCount);

	const TypeInfo* retType = fn->GetSigneture()->GetReturnType();
	const TypeInfo* selfType = fn->GetSigneture()->GetSelfType();

	bool returns = retType ? true : false;
	bool method  = selfType ? true : false;


	Variable self;
	unsigned int offset = 1;
	if (method)
	{
		// Allocate space for the self value on the stack
		self = Variable(alloca(selfType->Size()), selfType);
		self.FromLua(L, 1);
		offset++;
	}

	Variable ret;
	if (returns)
	{
		// Allocate space for the return value on the stack
		ret = Variable(alloca(retType->Size()), retType);
	}
	// Get the return type information from the functor

	// Allocate space for the arguments and placement new defaults
	Variable *stackArgs = (Variable *)alloca(sizeof(Variable) * fn->GetSigneture()->ArgCount());

	// new all of the arguments
	for (unsigned int i = 0; i < fn->GetSigneture()->ArgCount(); ++i)
	{
		void* data = alloca(fn->GetSigneture()->GetArg(i)->Size());
		const TypeInfo* type = fn->GetSigneture()->GetArg(i);
		new (stackArgs + i) Variable( data, type );
		stackArgs[i].FromLua(L, i + offset);

	}

	// Call function
	(*fn)(self, ret, stackArgs, fn->GetSigneture()->ArgCount());

	//// delete new all of the arguments
	//for (unsigned int i = 0; i < fn->GetSigneture()->ArgCount(); ++i)
	//{
	//	_freea(stackArgs[i].GetVoidPtr());
	//}
	//_freea(stackArgs);

	if(returns)
	{
		ret.ToLua(L);
		return 1;
	}
	// Number of returned arguments to Lua
	return 0;
}

void BindFunctionToLua(lua_State *L, Function *fn, const char *name)
{
	const TypeInfo* selfType = fn->GetSigneture()->GetSelfType();
	if (selfType == nullptr)
	{
		lua_pushlightuserdata(L, fn);
		lua_pushcclosure(L, GenericFunc, 1);
		lua_setglobal(L, name);
	}
	else //its a method
	{
		luaL_getmetatable(L, selfType->MetaTableName()); //a
		lua_pushstring(L, name); //b
		lua_pushlightuserdata(L, fn); //c
		lua_pushcclosure(L, GenericFunc, 1);
		lua_settable(L, -3); //a[b] = c
		lua_settop(L, 0);
	}
}
void InitEnvironment(lua_State* L)
{
	REGISTER_META
		const TypeInfo* type = TypeInfo::Head();

	while (type != nullptr)
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

			//--------------------------------------------- set mt_name propery
			luaL_getmetatable(L, type->MetaTableName()); //a
			lua_pushstring(L, "mt_name"); //b
			lua_pushstring(L, type->MetaTableName()); //b
			lua_settable(L, -3); //a[b] = c
			lua_settop(L, 0);

		}
		else
			lua_pop(L, 1);

		type = type->Next();
	}

}