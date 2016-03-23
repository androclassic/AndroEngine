#pragma once
#include "Function.h"


// Generic function call from Lua
int GenericFunc(lua_State *L);

void BindFunctionToLua(lua_State *L, Function *fn, const char *name);

void InitEnvironment(lua_State* L);

#define lua_bind(L, func) 	static Function gen_##func = make_gen(func);\
								BindFunctionToLua(L, &gen_##func, #func);

#define lua_bind_explicit(L, func, name) 	static Function gen_##name = make_gen(func);\
								BindFunctionToLua(L, &gen_##name, #name);


#define lua_bind_member(L, object, func) 	static Function gen_##object_##func = make_gen(object::func);\
										BindFunctionToLua(L, &gen_##object_##func, #func);
