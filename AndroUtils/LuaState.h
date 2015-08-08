#pragma once
#include<iostream>
#include<map>
#include "LuaBinding.h"

class Lua_State
{
	lua_State *L;
public:
	Lua_State() : L(luaL_newstate()) 
	{
		InitEnvironment(L);
	}

	Lua_State(Lua_State const&) = delete;
	Lua_State& operator=(Lua_State const&) = delete;
	Lua_State(Lua_State&&);

	~Lua_State() {
		lua_close(L);
	}

	// implicitly act as a lua_State pointer
	inline operator lua_State*() {
		return L;
	}
};

static void open_libs(lua_State *L)
{
	luaopen_io(L);
	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);
	luaL_openlibs(L);
}

static int execute_program(lua_State *L)
{
	// make a short function to make program behaviour more clear
	return lua_pcall(L, 0, LUA_MULTRET, 0);
}

static void report_errors(lua_State *L, const int status)
{
	if (status != 0) {
		const char* msg = lua_tostring(L, -1);

		std::wstring wc(strlen(msg), L'#');
		mbstowcs(&wc[0], msg, strlen(msg));

		TRACE(wc.c_str());
		lua_pop(L, 1); // remove error message
	}
}



