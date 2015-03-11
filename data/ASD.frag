#version 330



uniform sampler2D renderTexture;
uniform sampler2D image;


uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;
smooth in vec2 texcoord;
smooth in vec4 vVaryingShadowPos;
smooth in vec4 vOutColor;

out vec4 vFragColor;
void main(void)
{
	// Dot product gives us diffuse intensity
	float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));
	// Multiply intensity by diffuse color, force alpha to 1.0
	vFragColor = diff * diffuseColor;
	// Add in ambient light
	if(length(vOutColor) > 0)
		vFragColor +=  vOutColor;
	else
		vFragColor +=  ambientColor;
		
	// Specular Light
	vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
	normalize(vVaryingNormal)));
	float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
	// If the diffuse light is zero, don’t even bother with the pow function
	if(diff != 0) 
	{
		float fSpec = pow(spec, 128.0);
		vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
	
		//Add the attenuation
		
		float dist = length(vVaryingLightDir);
		
        float att = 1.0 / (0.05 +
                0.01 * dist +
                0.0001 * dist * dist);
        vFragColor *= att ;
    
	}
		vec2 poissonDisk[4] = vec2[](
		  vec2( -0.94201624, -0.39906216 ),
		  vec2( 0.94558609, -0.76890725 ),
		  vec2( -0.094184101, -0.92938870 ),
		  vec2( 0.34495938, 0.29387760 )
		);
		
		float visibility = 1.0;
		for (int i=0;i<4;i++)
		{
			float val = texture2D(renderTexture, vVaryingShadowPos.xy / vVaryingShadowPos.w + poissonDisk[i]/700.0).r; 
			float fragDepth = vVaryingShadowPos.z / vVaryingShadowPos.w; 
			if(val <= fragDepth  )
				visibility-=0.2;
		  
		}
			vFragColor =vFragColor * visibility;

	if(texture2D(image, texcoord.st).r > 0 || 
	   texture2D(image, texcoord.st).g > 0 || 
	   texture2D(image, texcoord.st).b > 0)
	{	
		vFragColor =vFragColor * texture2D(image, texcoord.st) ;

	}
	
}





