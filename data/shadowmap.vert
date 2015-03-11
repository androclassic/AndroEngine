#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;

layout(location = 0) in vec3 vVertex;
 
void main(){
 gl_Position  = projectionMatrix * viewMatrix * worldMatrix * vec4(vVertex,1);
}