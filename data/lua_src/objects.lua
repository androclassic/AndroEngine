require "data.lua_src.game_entity"

-------------------------------------------------
 Entity
 {
	Name  = "GroundPlane",
	Model = "data/cube2.obj",
	Material = 	{
					Texture = "doza.jpg",
					Shader  = "Deferred",
				},
	Physics = 	{
					primitiveType = PrimitiveType.PLANE,
					normal = Vector3(0,1,0),
					offset = -10,
					mass   = PRIM_MAX_MASS,
				},
			
	OnInit = function( o )			
		o:SetPosition(-50, -10, -50)
		o:SetScale(100, 0.1, 100)
	end,
}	

-------------------------------------------------

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
		o:SetScale(5,5,5)
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
		o:SetScale(5,5,5)
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
		o:SetScale(5,5,5)
	end,
	
	OnDestroy = function( o )
		Print_C(o.Name.." Destroyed !")
	end,

}
