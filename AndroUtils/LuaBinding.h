#pragma once
#include "Function.h"


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

#define lua_bind(L, func) 	static Function gen_##func = make_gen(func);\
								BindFunctionToLua(L, &gen_##func, #func);

#define lua_bind_member(L, object, func) 	static Function gen_##object_##func = make_gen(object::func);\
										BindFunctionToLua(L, &gen_##object_##func, #func);
