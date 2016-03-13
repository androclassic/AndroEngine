#pragma once
#include<iostream>
#include<map>
#include "LuaBinding.h"
#include "../Utils/Singleton.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/TextFileLoader.h"

class Lua_State
{
public:

	SINGLETON(Lua_State)

	bool Init();

	

	// implicitly act as a lua_State pointer
	inline operator lua_State*() {
		return L;
	}


	int execute_program(const std::string& program)
	{

		auto file = m_luaResourceManager.Load<andro::TextFile>(program, NULL);
		int status = luaL_dostring(L, file->GetContent().c_str());
		report_errors(status);

		return status;
    }
private:
	void report_errors(const int status)
	{
		if (status != 0) 
        {
			const char* msg = lua_tostring(L, -1);

			std::wstring wc(strlen(msg), L'#');
			mbstowcs(&wc[0], msg, strlen(msg));

			TRACE(wc.c_str());
			lua_pop(L, 1); // remove error message
			ASSERT(FALSE);
		}
	}

private:
	lua_State *L;
	andro::ResourceManager m_luaResourceManager;

};






