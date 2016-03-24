#include "EffectLib.h"
#include "Program.h"

namespace TakeTwo
{
	void EffectDesc::FromLua(lua_State * L, int index, Variable * ref)
	{
		ASSERT(lua_istable(L, index));

		EffectDesc* ref_effect = new ((EffectDesc*)ref->GetVoidPtr())EffectDesc();

		lua_getfield(L, index, "Name");
		lua_getfield(L, index, "vertex_shader");
		lua_getfield(L, index, "pixel_shader");

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
	
	void EffectLibrary::AddEffect( EffectDesc pEffectdesc, BlendDesc pBlendDesc, DepthStencilDesc pDepthStenciDesc)
	{
		EffectLibrary* lib = GetInstance();
		if(lib->m_effects[pEffectdesc.name] == nullptr)
			lib->m_effects[pEffectdesc.name] = new Program(pEffectdesc.vertex_shader, pEffectdesc.pixel_shader);
		else
		{
			ASSERT(false); //the effect already exists
		}
	}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

	BlendOperation BlendDesc::BlendOperationDecode(const unsigned int pLua_code)
	{
		switch (pLua_code)
		{
		case 1:			return (BLEND_OPERATION_ADD);

		case 2:			return (BLEND_OPERATION_SUBTRACT);

		case 3:			return (BLEND_OPERATION_REV_SUBTRACT);

		case 4:			return (BLEND_OPERATION_MIN);

		case 5:			return (BLEND_OPERATION_MAX);
		default:
						TRACE(L"UNKNOWN OPERATION");
						ASSERT(false);// unknown operation
						return BLEND_OPERATION_INVALID;
		}
	}

	BlendOption BlendDesc::BlendOptionDecode(const unsigned int pLua_code)
	{
		switch (pLua_code)
		{
		case 1:			return (BLEND_OPTION_ZERO);
		case 2:			return (BLEND_OPTION_ONE);
		case 3:			return (BLEND_OPTION_SRC_COLOUR);
		case 4:			return (BLEND_OPTION_INV_SRC_COLOUR);
		case 5:			return (BLEND_OPTION_SRC_ALPHA);
		case 6:			return (BLEND_OPTION_INV_SRC_ALPHA);
		case 7:			return (BLEND_OPTION_DEST_ALPHA);
		case 8:			return (BLEND_OPTION_INV_DEST_ALPHA);
		case 9:			return (BLEND_OPTION_DEST_COLOUR);
		case 10:		return (BLEND_OPTION_INV_DEST_COLOUR);
		case 11:		return (BLEND_OPTION_SRC_ALPHA_SAT);
		case 12:		return (BLEND_OPTION_BLEND_FACTOR);
		case 13:		return (BLEND_OPTION_INV_BLEND_FACTOR);
		case 14:		return (BLEND_OPTION_SRC1_COLOR);
		case 15:		return (BLEND_OPTION_INV_SRC1_COLOUR);
		case 16:		return (BLEND_OPTION_SRC1_ALPHA);
		case 17:		return (BLEND_OPTION_INV_SRC1_ALPHA);


		default:
			ASSERT(false);
			return BLEND_OPTION_INVALID;
		}
	}
	void BlendDesc::FromLua(lua_State * L, int index, Variable * ref)
	{
		ASSERT(lua_istable(L, index));

		BlendDesc* ref_blend = new ((BlendDesc*)ref->GetVoidPtr())BlendDesc();

		lua_getfield(L, index, "enable");
		ref_blend->m_enable = (bool)lua_toboolean(L, -1);

		lua_getfield(L, index, "src_colour");
		lua_getfield(L, index, "dest_colour");
		lua_getfield(L, index, "op_colour");


		ref_blend->m_src_colour = BlendOptionDecode(lua_tointeger(L, -3));
		ref_blend->m_dst_colour = BlendOptionDecode(lua_tointeger(L, -2));
		ref_blend->m_op_colour = BlendOperationDecode(lua_tointeger(L, -1));

		lua_getfield(L, index, "src_alpha");
		lua_getfield(L, index, "dest_alpha");
		lua_getfield(L, index, "op_alpha");

		ref_blend->m_src_alpha = BlendOptionDecode(lua_tointeger(L, -3));
		ref_blend->m_dst_alpha = BlendOptionDecode(lua_tointeger(L, -2));
		ref_blend->m_op_alpha = BlendOperationDecode(lua_tointeger(L, -1));

	}
	void BlendDesc::ToLua(lua_State * L, Variable & var)
	{
		ASSERT(false); //not implemented
	}

	ComparisonMode DepthStencilDesc::ComparisonModeDecode(const unsigned int pLua_code)
	{
		switch (pLua_code)
		{
			case 1: return COMPARISON_NEVER;
			case 2: return COMPARISON_ALWAYS;
			case 3: return COMPARISON_LESS;
			case 4: return COMPARISON_LESSEQUAL;
			case 5: return COMPARISON_EQUAL;
			case 6: return COMPARISON_NOTEQUAL;
			case 7: return COMPARISON_GREATER;
			case 8: return COMPARISON_GREATEREQUAL;
			default: return COMPARISON_INVALID;
		}
	}

	void DepthStencilDesc::FromLua(lua_State * L, int index, Variable * ref)
	{
		ASSERT(lua_istable(L, index));

		DepthStencilDesc* ref_depth_st = new ((DepthStencilDesc*)ref->GetVoidPtr())DepthStencilDesc();

		lua_getfield(L, index, "depthEnable");
		lua_getfield(L, index, "depthWrite");
		lua_getfield(L, index, "depthFunc");

		ref_depth_st->m_depthEnable  = (bool)lua_toboolean(L, -3);
		ref_depth_st->m_depthWrite	 = (bool)lua_toboolean(L, -2);
		ref_depth_st->m_depthFunc	 = ComparisonModeDecode(lua_tointeger(L, -1));
	}
	void DepthStencilDesc::ToLua(lua_State * L, Variable & var)
	{
		ASSERT(false); //not implemented
	}

}