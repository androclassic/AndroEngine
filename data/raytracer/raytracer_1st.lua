


require "data.raytracer.raytracer_inc"


		
function init()
	math.randomseed( os.time() )

	local light_intensity = 7

	local bgColour = vec3(0.0024,0.0024,0.0024)
	local cameraPosition = vec3(5, 1.5, 3.5)
	local cameraLook = vec3(0,0,-1)
	local width = 600	local height = 300	local samples = 10
--	local width = 1980	local height = 1080	local samples = 1000

	InitFrame(vec3(width,height,samples), bgColour, cameraPosition, cameraLook)



			
-- add lights to the scene		
		CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LIGHT,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 0, g = 0, b = light_intensity },
			 Position = {x = -2, y = 4, z = 2 },
--			 Roughness = 0.32,
			 Size = 0.7
			}
		CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LIGHT,
			 Texture = Texture.CONSTANT,
			 Colour = {r = light_intensity, g = light_intensity, b = 0 },
			 Position = {x = 2, y = 4, z = -2 },
--			 Roughness = 0.32,
			 Size = 0.7
			}
		
		CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LIGHT,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 0, g = light_intensity, b = 0 },
			 Position = {x = -2, y = 4, z = -2 },
--			 Roughness = 0.32,
			 Size = 0.7
			}

		CreateObject
			{
			 Type = ObjectType.RECT_XZ,
			 Material=MaterialType.LIGHT,
			 Texture = Texture.CONSTANT,
			 Colour = {r = light_intensity, g = 0, b = 0 },
			 Position = {x = 2, y = 4, z = 2 },
--			 Roughness = 0.32,
			 Size = 0.7
			}
-- //////////////////////////////////////////////////////////////////

		CreateObject
			{
			 Type = ObjectType.SPHERE,
			 Material=MaterialType.LAMBERTIAN,
			 Texture = Texture.NOISE,
			 Colour = {r = 0.9, g = 0.9, b = 0.9 },
			 Position = {x = 0, y = -1000, z = -1 },
--			 Roughness = 0.32,
			 Size = 1000
			}

			
			
	
		local i = 1
		for a = -11,  11  do
		
			for b = -11, 11 do

				local choose_mat = math.random()
				local center= { x = a + 0.9* math.random(), y = 0.2, z = b * math.random()}
				
				if (Length({x = center.x - 4, y = center.y - 0.2, z = center.z}) > 0.9) then
					if (choose_mat < 0.8) then --difuse
						CreateObject
							{
							 Type = ObjectType.SPHERE,
							 Material=MaterialType.LAMBERTIAN,
							 Texture = Texture.CONSTANT,
							 Colour = {r = math.random()* math.random(), g = math.random()* math.random(), b = math.random()* math.random() },
							 Position = center,
				--			 Roughness = 0.32,
							 Size = 0.2
							}
					elseif (choose_mat < 0.95) then --METAL
						CreateObject
							{
							 Type = ObjectType.SPHERE,
							 Material=MaterialType.METAL,
							 Texture = Texture.CONSTANT,
							 Colour = {r =0.5*(1 + math.random()), g = 0.5*(1 + math.random()), b = 0.5*(1 + math.random()) },
							 Position = center,
							 Roughness = 0.5*(1 + math.random()),
							 Size = 0.2
							}
	
					else
						CreateObject
							{
							 Type = ObjectType.SPHERE,
							 Material=MaterialType.DIELECTRIC,
							 Texture = Texture.CONSTANT,
							 Colour = {r = 0, g = 0, b = 0 }, --not used
							 Position = center,
							 Roughness = 1.5, --refractive index
							 Size = 0.2
							}
					end

				end
			end
			
		end
		
		
		--////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		CreateObject
			{
			 Type = ObjectType.SPHERE,
			 Material=MaterialType.DIELECTRIC,
			 Texture = Texture.CONSTANT,
			 Colour = {r = 0, g = 0, b = 0 }, --not used
			 Position = {x = 0, y = 1, z = 0.5 },
			 Roughness = 1.5, --refractive index
			 Size = 1.0
			}
			
			CreateObject
				{
				 Type = ObjectType.SPHERE,
				 Material=MaterialType.LAMBERTIAN,
				 Texture = Texture.CONSTANT,
				 Colour = {r = 0.4, g = 0.2, b = 0.1 },
				 Position = { x = -4, y = 1, z = 1 } ,
	--			 Roughness = 0.32,
				 Size = 1
				}
		
			CreateObject
				{
				 Type = ObjectType.BOX,
				 Material=MaterialType.METAL,
				 Texture = Texture.CONSTANT,
				 Colour = {r = 1, g = 1, b = 0.2 },
				 Position = { x = 4, y = 1, z = 3.5 } ,
				 Roughness = 0.8,
				 Size = 0.1
				}
				
			CreateObject
				{
				 Type = ObjectType.BOX,
				 Material=MaterialType.METAL,
				 Texture = Texture.CONSTANT,
				 Colour = {r = 0.7, g = 0.6, b = 0.2 },
				 Position = { x = 4, y = 1, z = 0 } ,
				 Roughness = 0.05,
				 Size = 1
				}
end
		
init()



