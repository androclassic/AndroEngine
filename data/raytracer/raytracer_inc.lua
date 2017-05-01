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
			BOX_FROM_PLANES = 5,
		}
		
MaterialType = 
		{
			LAMBERTIAN = 0,
			DIELECTRIC = 1,
			METAL = 2,
			LIGHT = 3,
			ISOTROPIC = 4,
		}		

Length = function(v1)	
	return  math.sqrt(v1.x*v1.x + v1.y * v1.y + v1.z * v1.z)
end