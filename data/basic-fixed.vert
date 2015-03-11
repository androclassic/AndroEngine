#version 120

uniform mat4 projectionMatrix;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

attribute vec3 vVertex;
attribute vec4 vColor;
attribute vec2 vTextCoord;
attribute vec3 vNormal;


varying vec4 color;
varying vec2 texcoord;

void main(void) 
{
    vec4 pos = viewMatrix * worldMatrix * vec4(vVertex, 1.0);

	color = vColor;

	texcoord = vTextCoord;
	gl_Position = projectionMatrix * pos;	
}