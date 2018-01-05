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
	Name  = "Dwarf",
	Model = "data/dwarf.obj",
	Material = 	{
					Texture = "dwarf.jpg",
					Shader  = "Deferred",
				},
	
	OnInit = function( o )			
		o.index = dwarfIndex
		dwarfIndex = dwarfIndex + 1
		
		local x = radius * math.cos(angle * o.index)
		local y = radius * math.sin(angle * o.index)

		o:SetPosition(0 ,o.index * 5, 0)
		o:SetScale(0.05)
	end,
	
	OnUpdate = function( o )
		o.timestep = o.timestep + 0.0001

		local x = radius * math.cos(angle * (o.index + o.timestep))
		local y = radius * math.sin(angle * (o.index + o.timestep))
		o:SetPosition(x , 0 , y)
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,
	
	timestep = 0,
	index = 0

 }	


 Entity
 {
	Name  = "Dwarf2",
	Model = "data/dwarf.obj",
	Material = 	{
					Texture = "dwarf2.jpg",
					Shader  = "Deferred",
				},
	Physics = {
					primitiveType = PrimitiveType.BOX,
					halfSize = Vector3(1,1,1),
					mass   = 0.2,
				},
	
	OnInit = function( o )			
		o:SetScale(1)
		o.index = dwarfIndex
		dwarfIndex = dwarfIndex + 1
		
		local x = radius * math.cos(angle * o.index)
		local y = radius * math.sin(angle * o.index)

		o:SetPosition(0 ,o.index * 5, 0)
		o:SetScale(0.03)
	end,
	
	OnUpdate = function( o )
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,
	
}
---------------------------------------------------------------------