#include "EffectLib.h"
#include "Program.h"

namespace TakeTwo
{
	void EffectDesc::FromLua(lua_State * L, int index, Variable * ref)
	{
		ASSERT(lua_istable(L, index));

		EffectDesc* ref_effect = new ((EffectDesc*)ref->GetVoidPtr())EffectDesc();

		lua_getfield(L, 1, "Name");
		lua_getfield(L, 1, "vertex_shader");
		lua_getfield(L, 1, "pixel_shader");

		const char *Name = luaL_checkstring(L, -3);
		const char *vertex_shader = luaL_checkstring(L, -2);
		const char *pixel_shader = luaL_checkstring(L, -1);

		ref_effect->name = Name;
		ref_effect->vertex_shader = vertex_shader;
		ref_effect->pixel_shader = pixel_shader;
	}

	void EffectDesc::ToLua(lua_State * L, Variable & var)
	{
		ASSERT(false); //not implemented
	}
//-------------------------------------------------------------------------------
	EffectLibrary::EffectLibrary() { }

	EffectLibrary::~EffectLibrary() 
	{
		for (std::map<std::string, Program*>::iterator iterator = m_effects.begin(); iterator != m_effects.end(); iterator++)
		{
			if (iterator->second != nullptr)
				delete iterator->second;
		}
		m_effects.clear();
	}

	Program* EffectLibrary::GetEffect(const std::string& name)
	{
		return m_effects[name]; 
	}
	
	void EffectLibrary::AddEffect(EffectDesc desc)
	{
		EffectLibrary* lib = GetInstance();
		if(lib->m_effects[desc.name] == nullptr)
			lib->m_effects[desc.name] = new Program(desc.vertex_shader, desc.pixel_shader);
		else
		{
			ASSERT(false); //the effect already exists
		}
	}

}