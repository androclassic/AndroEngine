#pragma once
#include <string>
#include "OGL_Platform.h"
#include "../../AndroUtils/Utils/Singleton.h"
#include "../../AndroUtils/Introspection/LuaState.h"

namespace TakeTwo
{
	class Program;
	class BlendDesc;
	class DepthStencilDesc;
	
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
		static void AddEffect( EffectDesc pEffectDesc, BlendDesc pBlendDesc, DepthStencilDesc pDepthStenciDesc);


	private:
		std::map<std::string, Program*> m_effects;
	};



	struct BlendDesc 
	{
		BlendOption				m_src_colour;
		BlendOption				m_dst_colour;
		BlendOperation			m_op_colour;

		BlendOption				m_src_alpha;
		BlendOption				m_dst_alpha;
		BlendOperation			m_op_alpha;
		bool					m_enable;

		static BlendOperation BlendOperationDecode(const unsigned int pLua_code);
		static BlendOption BlendOptionDecode(const unsigned int pLua_code);
		static void FromLua(lua_State * L, int index, Variable * ref);
		static void ToLua(lua_State * L, Variable & var);

	};

	struct DepthStencilDesc
	{
		bool			m_depthEnable;
		bool			m_depthWrite;
		ComparisonMode	m_depthFunc;

		static ComparisonMode ComparisonModeDecode(const unsigned int pLua_code);
		static void FromLua(lua_State * L, int index, Variable * ref);
		static void ToLua(lua_State * L, Variable & var);
	};
}


