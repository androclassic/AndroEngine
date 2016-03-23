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
	Name  = "CubeNormal",
	Model = "data/cube2.obj",
	Material = 	{
					Texture = "dwarf.jpg",
					Shader  = "SimpleTextureMap",
				},
	OnInit = function( o )			
		o:SetPosition(0, -10, 0)
		o:SetScale(5)
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,

}
---------------------------------------------
	

 Entity
 {
	Name  = "CubeDiffuse",
	Model = "data/cube2.obj",
	Material = 	{
					Texture = "dwarf.jpg",
					Shader  = "SimpleTextureMap",
				},
	OnInit = function( o )			
		o:SetPosition(5, -10, 0)
		o:SetScale(5)
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,

}
---------------------------------------------
	

 Entity
 {
	Name  = "CubeSpecular",
	Model = "data/cube2.obj",
	Material = 	{
					Texture = "dwarf.jpg",
					Shader  = "SimpleTextureMap",
				},
	OnInit = function( o )			
		o:SetPosition(10, -10, 0)
		o:SetScale(5)
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,

}

----------------------------------------------------------------

 Entity
 {
	Name  = "Dwarf2",
	Model = "data/dwarf.obj",
	Material = 	{
					Texture = "dwarf2.jpg",
					Shader  = "Deferred",
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