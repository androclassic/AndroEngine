
require "data.lua_src.effects.definitions"

effect = function( desc_table )
	
	local blend_state_desc = blend_state_process(desc_table.blend_state)
	local depthstencil_state_desc = depthstencil_state_process(desc_table.depthstencil_state)
	
	AddEffectLib(desc_table, blend_state_desc, depthstencil_state_desc)
end




--	 blend_desc
--	{
--		src_colour
--		dest_colour
--		op_colour
--		
--		src_alpha
--		dest_alpha
--		op_alpha
--	}

blend_state_process = function ( desc_table )
   local state = {}
   
   state.enable		= desc_table.enable or false
   state.src_colour = desc_table.src_colour or  BlendOption.SRC_ALPHA
   state.dest_colour = desc_table.dest_colour or  BlendOption.INV_SRC_ALPHA
   state.op_colour	= desc_table.op_colour  or BlendOperation.ADD
   
   state.src_alpha = desc_table.src_alpha 	or  BlendOption.ONE
   state.dest_alpha = desc_table.dest_alpha or  BlendOption.ZERO
   state.op_alpha	= desc_table.op_alpha  	or BlendOperation.ADD
   
   return state
end






--	 deptstencil_desc
--	{
--		depthEnable
--		depthWrite
--		depthFunc
--	}

depthstencil_state_process = function ( desc_table )
   local state = {}
   
   state.depthEnable 	= desc_table.depthEnable or false
   state.depthWrite 	= desc_table.depthWrite or false
   state.depthFunc 		= desc_table.depthFunc or  ComparisonMode.LESSEQUAL
   
   return state
end





