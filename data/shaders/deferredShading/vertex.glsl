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