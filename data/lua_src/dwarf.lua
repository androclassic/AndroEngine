require "data.lua_src.game_entity"

--GLOBALS
---------------------------------------------
dwarfNumbers = 30
dwarfIndex = 1

angle = 2 * 3.14 / dwarfNumbers;
radius =  15;	
---------------------------------------------
	

 Entity
 {
	Name  = "Dwarf1",
	Model = "data/sphere.obj",
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

		if o.height > 20 or o.height  < 4 then
			o.grow_dorection = o.grow_dorection * -1
		end
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,
	
	
	height =  4,
	grow_dorection = 1,
	grow_speed = 0.0001,
}

----------------------------------------------------------------

 Entity
 {
	Name  = "Dwarf2",
	Model = "data/dwarf.obj",
	Material = 	{
					Texture = "dwarf2.jpg",
					Shader  = "deferredshading",
				},
				
	OnInit = function( o )			
		o:SetScale(1)
		o.index = dwarfIndex
		dwarfIndex = dwarfIndex + 1
		
		local x = radius * math.cos(angle * o.index)
		local y = radius * math.sin(angle * o.index)

		o:SetPosition(x ,-10 , y)
		o:SetScale(0.08)
	end,
	
	OnUpdate = function( o )
		o.timestep = o.timestep + 0.001

		local x = radius * math.cos(angle * (o.index + o.timestep))
		local y = radius * math.sin(angle * (o.index + o.timestep))
		o:SetPosition(x ,(o.index - 35) / 2 , y)
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,
	
	timestep = 0,
	scale = 0,
	index = 0
}
---------------------------------------------------------------------