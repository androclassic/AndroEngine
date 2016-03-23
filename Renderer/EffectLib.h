#pragma once
#include <string>
#include "../../AndroUtils/Utils/Singleton.h"
#include "../../AndroUtils/Introspection/LuaState.h"

namespace TakeTwo
{
	class Program;
	
	struct EffectDesc
	{
		std::string name;
		std::string vertex_shader;
		std::string pixel_shader;

		static void FromLua(lua_State * L, int index, Variable * ref);
		static void ToLua(lua_State * L, Variable & var);
	};

	class EffectLibrary
	{
	public:
		SINGLETON(EffectLibrary);

		Program* GetEffect(const std::string& name);
		static void AddEffect(EffectDesc desc);


	private:
		std::map<std::string, Program*> m_effects;
	};
}


