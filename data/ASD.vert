#version 330
// Incoming per vertex... position and normal
in vec3 vVertex;
in vec3 vNormal;
in vec4 vColor;
in vec2 vTextCoord;


// Set per batch
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec3 vLightPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 worldMatrix;

smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;
smooth out vec2 texcoord;
smooth out vec4 vVaryingShadowPos;
smooth out vec4 vOutColor;


void main(void)
{
	 mat4 modelview = viewMatrix * worldMatrix ;
		
	 // Get surface normal in eye coordinates
	  vVaryingNormal = mat3(modelview)  * vNormal;
	 // Get vertex position in eye coordinates
	 vec4 vPosition4 = modelview  * vec4(vVertex, 1);
	 vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
	 vec3 lightPostition = vec4(viewMatrix * vec4(vLightPosition,1)).xyz;
	 
	 // Get vector to light source
	  vVaryingLightDir = lightPostition - vPosition3;

	 //Don’t forget to transform the geometry!
	//vVaryingColor = vColor;
	texcoord = vTextCoord;

	mat4 bias =mat4(	vec4(0.5, 0.0, 0.0, 0.0),
				vec4(0.0, 0.5, 0.0, 0.0),
				vec4(0.0, 0.0, 0.5, 0.0),
				vec4(0.5, 0.5, 0.5, 1.0) );
			
	vVaryingShadowPos = bias * projectionMatrix * lightViewMatrix * worldMatrix * vec4(vVertex,1);
		gl_Position = projectionMatrix *  viewMatrix * worldMatrix * vec4(vVertex,1);
		vOutColor = vColor;

}