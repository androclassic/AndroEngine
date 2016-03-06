#include "LuaState.h"
Lua_State::Lua_State(void)
{

}

bool Lua_State::Init()
{
	static bool init = false;

	if (!init)
	{
		L = luaL_newstate();
		InitEnvironment(L);
		luaopen_io(L);
		luaopen_base(L);
		luaopen_table(L);
		luaopen_string(L);
		luaopen_math(L);
		luaL_openlibs(L);
		init = true;
	}


	return true;
}

Lua_State::~Lua_State()
{
	lua_close(L);
}