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
	 mat4 modelview = worldMatrix * viewMatrix;
		
	 // Get surface normal in eye coordinates
	  vVaryingNormal = vNormal * mat3(modelview);
	 // Get vertex position in eye coordinates
	 vec4 vPosition4 = vec4(vVertex, 1) * modelview ;
	 vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
	 vec3 lightPostition = vec4(vec4(vLightPosition,1) * viewMatrix).xyz;
	 
	 // Get vector to light source
	  vVaryingLightDir = lightPostition - vPosition3;

	 //Don’t forget to transform the geometry!
	//vVaryingColor = vColor;
	texcoord = vTextCoord;

	mat4 bias =mat4(	vec4(0.5, 0.0, 0.0, 0.5),
						vec4(0.0, 0.5, 0.0, 0.5),
						vec4(0.0, 0.0, 0.5, 0.5),
						vec4(0.0, 0.0, 0.0, 1.0) );

						
	vVaryingShadowPos =vec4(vVertex,1) * worldMatrix * lightViewMatrix * projectionMatrix * bias ;
	gl_Position = vec4(vVertex,1) * worldMatrix * viewMatrix * projectionMatrix;
	vOutColor = vColor;

}