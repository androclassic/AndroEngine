


Texture =
		{
			CONSTANT = 0,
			NOISE  	 = 1,
		}

ObjectType =
		{
			BOX = 0,
			SPHERE = 1,
			RECT_XY= 2,
			RECT_YZ = 3,
			RECT_XZ = 4,
		}
		
MaterialType = 
		{
			LAMBERTIAN = 0,
			DIELECTRIC = 1,
			METAL = 2,
			LIGHT = 3,
		}		

Length = function(v1)	
	return  math.sqrt(v1.x*v1.x + v1.y * v1.y + v1.z * v1.z)
end

		
function init()
--	math.randomseed( os.time() )
	math.randomseed( 9741 )
	local light_intensity = 7

	local bgColour = vec3(0.32,0.25,0.3)
	local cameraPosition = vec3(-2.3,0.65,0)
	local cameraLook = vec3(0,0,0)
--	local width = 1980	local height = 1080	local samples = 1
--	local width = 1280	local height = 640	local samples = 1
	local width = 400	local height = 300	local samples = 1

	InitFrame(vec3(width,height,samples), bgColour, cameraPosition, cameraLook)
-- add lights to the scene		
		CreateObject
			{
			 Type = ObjectType.SPHERE,
			 Material=MaterialType.LIGHT,
			 Texture = Texture.CONSTANT,
			 Colour = {r = light_intensity, g = light_intensity, b = light_intensity },
			 Position = {x = 6, y = 3, z = 17 },
--			 Roughness = 0.32,
			 Size = 1
			}
		
	
		local size = 0.3
	
		local i = 1
		for a = -2.1,  2.1, size  do
		
			local row_colour =  {r = 184 / 255, g=115 /255, b = 51/255 }
			for b = -2.1, 2.1 , size do

				local position = { x = b , y = 0.2, z = a}
				
					CreateObject
						{
						 Type = ObjectType.BOX,
						 Material=MaterialType.METAL,
						 Texture = Texture.CONSTANT,
--						Colour = {r = 1, g = 215/255, b = 0 },
						 Colour = row_colour,
						 Position = position,
						 Roughness = 0.06,
						 Size = { x = size*1.1/2, y = size*1.1/4 + (size*1.1/2) * math.random(), z = size*1.1/2 } 
						}
	

			end
			
		end


		for a = -2.1,  2.1, size  do
		
			local row_colour =  {r = 184 / 255, g=115 /255, b = 51/255 }
			for b = -2.1, 2.1 , size do

				local position = { x = b , y = 1.5, z = a}
				
					CreateObject
						{
						 Type = ObjectType.BOX,
						 Material=MaterialType.METAL,
						 Texture = Texture.CONSTANT,
--						Colour = {r = 1, g = 215/255, b = 0 },
						 Colour = row_colour,
						 Position = position,
						 Roughness = 0.06,
						 Size = { x = size*1.1/2, y = size*1.1/4 + (size/2) * math.random(), z = size*1.1/2 } 
						}
	

			end
			
		end
		
		
		--////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			CreateObject
				{
				 Type = ObjectType.SPHERE,
				 Material=MaterialType.DIELECTRIC,
				 Texture = Texture.CONSTANT,
				 Colour = {r = 0.7, g = 0.6, b = 0.6 },
				 Position = { x = -1.5, y = 0.5, z = 0 } ,
				 Roughness = 1.5,
				 Size = 0.1
				}

			
			local cubeSize = 0.4
			local spheres = 3
			local sphereSize = cubeSize/spheres
			
			local center = { x = -1, y = 0.7, z = -1 } 
			for i=-spheres/2, spheres/2 do
				for j=-spheres/2, spheres/2 do
					for k=-spheres/2, spheres/2 do

						local offset = { x = i * sphereSize, y = j*sphereSize, z = k*sphereSize} 
						local pos = {x = center.x + offset.x, y = center.y + offset.y, z = center.z + offset.z }
						local colour =  {r =1, g = 0.9, b = 0 } 
						
						CreateObject
							{
							 Type = ObjectType.SPHERE,
							 Material=MaterialType.METAL,
							 Texture = Texture.CONSTANT,
							 Colour = colour,
							 Position = pos,
							 Roughness = 0.03,
							 Size = sphereSize * 0.5
							}
					end
				end
			end
			
end
		
init()



