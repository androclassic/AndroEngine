
require "data.lua_src.game_entity"



 Entity
 {
	Name  = "Dwarf1",
	Model = "data/dwarf.obj",
	Material = 	{
					Texture = "dwarf.jpg",
					Shader  = "SimpleTextureMap",
				},
				
	OnInit = function( o )			
		o:SetPosition(0, -10, 0)
		o:SetScale(1)
	end,
	
	OnUpdate = function( o )
		o:SetScale(o.height)
		o.height = o.height + o.grow_speed * o.grow_dorection

		if o.height > 0.3 or o.height  < 0.04 then
			o.grow_dorection = o.grow_dorection * -1
		end
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,
	
	
	height =  0.04,
	grow_dorection = 1,
	grow_speed = 0.0001,
}


objNumber = 30
game_objects = {}

local angle = 2 * 3.14 / objNumber;
local radius =  5;


local mat = 
	{
		Texture = "dwarf2.jpg",
		Shader  = "SimpleTextureMap",
	}
local obj = "data/dwarf.obj"
	
for i = 0, objNumber do


	local gameObject = CreateGameObject(mat, obj)
	local x = radius * math.cos(angle * i)
	local y = radius * math.sin(angle * i)

	gameObject:SetPosition(x ,-10 , y)
	gameObject:SetScale(0.08)
	table.insert(game_objects, gameObject)
end
	
