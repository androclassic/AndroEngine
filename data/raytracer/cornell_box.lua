

require "data.raytracer.raytracer_inc"



		
function init()
--	math.randomseed( os.time() )
	math.randomseed( 9741 )
	local light_intensity = 0.3

--	local bgColour = vec3(0.3,0.3,0.3)
	local bgColour = vec3(0, 0, 0)
	local cameraPosition = vec3(0,0, 1.5)
	local cameraLook = vec3(0,0,0)
--	local width = 1980	local height = 1080	local samples = 1
--	local width = 1280	local height = 640	local samples = 150
	local width = 400	local height = 300	local samples = 1000

	InitFrame(vec3(width,height,samples), bgColour, cameraPosition, cameraLook)


	
	-- light
	
			CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LIGHT,
			 Texture = Texture.CONSTANT,
			 Colour = {r = light_intensity, g = light_intensity, b = light_intensity/2 },
			 Position = {x = 0, y = 0.999, z =  0 },
--			 Roughness = 0.32,
			 Size = 0.5
			}

-- ///////////////////////////////////////////////////////////////////////////
	


			CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LAMBERTIAN,
			 Texture = Texture.CONSTANT,
			 Colour = {r = .5, g = .5, b = .5 },
			 Position = {x = 0, y = -1, z =  0 },
--			 Roughness = 0.32,
			 Size = 1
			}
			
			CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LAMBERTIAN,
			 Texture = Texture.CONSTANT,
			 Colour = {r = .5, g = .5, b = .5 },
			 Position = {x = 0, y = 1, z =  0 },
--			 Roughness = 0.32,
			 FlipNormals = true,
			 Size = 1
			}

			CreateObject
			{
			 Type = ObjectType.RECT_YZ,
			 Material=MaterialType.LAMBERTIAN,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 0, g = 1, b = 0 },
			 Position = {x = 1, y = 0, z =  0 },
--			 Roughness = 0.32,
			 FlipNormals = true,
			 Size = 1
			}


			CreateObject
			{
			 Type = ObjectType.RECT_YZ,
			 Material=MaterialType.LAMBERTIAN,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 1, g = 0, b = 0 },
			 Position = {x = -1, y = 0, z =  0 },
--			 Roughness = 0.32,
			 Size = 1
			}


			CreateObject
			{
			 Type = ObjectType.RECT_XY,
			 Material=MaterialType.LAMBERTIAN,
			 Texture = Texture.CONSTANT,
			 Colour = {r = .5, g = .5, b = .5 },
			 Position = {x = 0, y = 0, z =  -1 },
--			 Roughness = 0.32,
			 Size = 1
			}

			
-- /////////////////////////////////////////////////////////////////////////

		CreateObject
			{
			 Type = ObjectType.BOX,
			 Material=MaterialType.METAL,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 1, g = 1, b = 0.2 },
			 Position = { x = -0.3, y = -0.7, z = 0 } ,
			 Roughness = 0.8,
			 Size = 0.3
			}

		CreateObject
			{
			 Type = ObjectType.SPHERE,
			 Material=MaterialType.METAL,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 0.2, g = 0.2, b = 0.2 },
			 Position = { x = -0.3, y = -.18, z = 0 },
			 Roughness = 0.01,
			 Size = 0.22
			}

			
			CreateObject
			{
			 Type = ObjectType.SPHERE,
			 Material=MaterialType.DIELECTRIC,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 0, g = 0, b = 0 }, --not used
			 Position = {x = 0.5, y = 0, z =  0 },
			 Roughness = 1.4, --refractive index
			 Size = 0.3
			}
			
			CreateObject
			{
			 Type = ObjectType.BOX_FROM_PLANES,
			 Material=MaterialType.ISOTROPIC,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 1, g = 0, b = 1 }, --not used
			 Position = {x = 0, y = -1.7, z =  0 },
			 Roughness = 1.2, --density
			 Size = { x = 1, y = 0.1, z = 1 },
			}
			
			
end
		
init()



