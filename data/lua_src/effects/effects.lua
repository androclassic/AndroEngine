-- ///////////////////////////////////////////////////////
require "data.lua_src.effects.effects_module"


normal_vs = [[
	#version 330 core

	uniform mat4 camera;
	uniform mat4 model;

	layout (location = 0) in vec3 vertPos;
	layout (location = 1) in vec3 vertNormal;
	layout (location = 3) in vec2 vertTexCoord;

	out vec3 fragVert;
	out vec2 fragTexCoord;
	out vec3 fragNormal;
	out vec3 cameraPosition;

	void main() {
		// Pass some variables to the fragment shader
		fragTexCoord = vertTexCoord;
		fragNormal = vertNormal;
		fragVert = vertPos;
		cameraPosition = -camera[3].xyz * mat3(camera);

		// Apply all matrix transformations to vert
		gl_Position = camera * model * vec4(vertPos,1);
	}
]]

simple_texture_map_ps = [[
	#version 330

	uniform mat4 model;

	// material settings
	uniform sampler2D u_texture;
	uniform float u_shininess;
	uniform vec4 u_color_specular;
	uniform vec4 u_color_diffuse;
	uniform int u_textures_count;

	uniform struct Light {
		vec3 position;
		vec3 intensities; //a.k.a the color of the light
		float attenuation;
		float ambientCoefficient;
	} light;

	in vec2 fragTexCoord;
	in vec3 fragNormal;
	in vec3 fragVert;
	in vec3 cameraPosition;

	out vec4 finalColor;

	void main() {

		vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
		vec3 surfacePos = vec3(model * vec4(fragVert, 1));
		vec4 surfaceColor = u_color_diffuse;
		
		if( u_textures_count > 0 )
			surfaceColor = texture(u_texture, fragTexCoord);
			
		vec3 surfaceToLight = normalize(light.position - surfacePos);
		vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

		//ambient
		vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

		//diffuse
		float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
		vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

		//specular
		float specularCoefficient = 0.0;
		if(diffuseCoefficient > 0.0)
			specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), 1 - u_shininess);
		vec3 specular = specularCoefficient * u_color_specular.rgb * light.intensities;

		//attenuation
		float distanceToLight = length(light.position - surfacePos);
		float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

		//linear color (color before gamma correction)
		vec3 linearColor = ambient + attenuation*(diffuse + specular);

		//final color (after gamma correction)
		vec3 gamma = vec3(1.0/1.8);
		finalColor = vec4(pow(linearColor, gamma), 1.2f);
		//finalColor = vec4(surfaceColor.rgb, 1.0f);
	}
]]

deferred_ps = [[

	#version 330

	uniform mat4 model;

	// material settings
	uniform sampler2D u_texture;
	uniform float u_shininess;
	uniform vec4 u_color_specular;
	uniform vec4 u_color_diffuse;
	uniform int u_textures_count;

	uniform struct Light {
		vec3 position;
		vec3 intensities; //a.k.a the color of the light
		float attenuation;
		float ambientCoefficient;
	} light;

	in vec2 fragTexCoord;
	in vec3 fragNormal;
	in vec3 fragVert;
	in vec3 cameraPosition;

	layout(location = 0) out vec4 out_diffuse;
	layout(location = 1) out vec4 out_normals;
	layout(location = 2) out vec4 out_specular;


	void main() {

		vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
		vec3 surfacePos = vec3(model * vec4(fragVert, 1));
		vec4 surfaceColor = u_color_diffuse;
		
		if( u_textures_count > 0 )
			surfaceColor = texture(u_texture, fragTexCoord);
			
		out_diffuse = surfaceColor;
		out_specular = vec4(surfacePos,1);
		out_normals = vec4(normal,1);;
	}
]]



fullscreen_quad = [[

	#version 330 core

	void main() {
	    // id | x  | y
		//----+----+---
		// 0  | 0  | 0
		//----+----+---
		// 1  | 1  | 0
		//----+----+---
		// 2  | 0  | 1
		//----+----+---
		// 3  | 1  | 1

		int x = gl_VertexID &  1;
		int y = gl_VertexID >> 1;

		vec4 output = vec4( x * 2 - 1, y * 2 - 1, 0.5, 1 );
		gl_Position = output;
	}
]]

deferred_resolve_ps = [[

	#version 330
	uniform sampler2D u_diffuse;
	uniform sampler2D u_normals;
	uniform sampler2D u_specular;

	layout(location = 0) out vec4 out_value;


	void main() 
	{
		vec2 texcoord = (gl_FragCoord.xy ) / vec2(1024, 768); 
		vec4 diffuse  = texture(u_diffuse , texcoord);
		vec4 normals  = texture(u_normals , texcoord);
		vec4 specular = texture(u_specular, texcoord);

		out_value = diffuse + specular;
	}
]]


effect 
	{
		Name = "resolve_deferred",
		blend_state =
					{
						enable		= true,
						src_colour 	= BlendOption.SRC_ALPHA,
						dest_colour	= BlendOption.INV_SRC_ALPHA,
						op_colour	= BlendOperation.ADD,
						src_alpha   = BlendOption.ONE,
						dest_alpha  = BlendOption.ZERO,
						op_alpha    = BlendOperation.ADD,
					},
		depthstencil_state =
					{
						depthEnable = true,
						depthWrite 	= true,
						depthFunc 	= ComparisonMode.LESS,
					},
					
		vertex_shader	= fullscreen_quad,
		pixel_shader	= deferred_resolve_ps,
		
	}
	

effect 
	{
		Name = "Deferred",
		blend_state =
					{
						enable		= true,
						src_colour 	= BlendOption.SRC_ALPHA,
						dest_colour	= BlendOption.INV_SRC_ALPHA,
						op_colour	= BlendOperation.ADD,
						src_alpha   = BlendOption.ONE,
						dest_alpha  = BlendOption.ZERO,
						op_alpha    = BlendOperation.ADD,
					},
		depthstencil_state =
					{
						depthEnable = true,
						depthWrite 	= true,
						depthFunc 	= ComparisonMode.LESS,
					},
					
		vertex_shader	= normal_vs,
		pixel_shader	= deferred_ps,
		
	}
effect 
	{
		Name = "SimpleTextureMap",
		blend_state =
					{
						enable		= true,
						src_colour 	= BlendOption.SRC_ALPHA,
						dest_colour	= BlendOption.INV_SRC_ALPHA,
						op_colour	= BlendOperation.ADD,
						src_alpha   = BlendOption.ONE,
						dest_alpha  = BlendOption.ZERO,
						op_alpha    = BlendOperation.ADD,
					},
		depthstencil_state =
					{
						depthEnable = true,
						depthWrite 	= true,
						depthFunc 	= ComparisonMode.LESS,
					},

		vertex_shader	= normal_vs,
		pixel_shader	= simple_texture_map_ps,
	}