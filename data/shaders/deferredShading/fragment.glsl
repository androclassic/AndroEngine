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
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normals;


void main() {

    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = u_color_diffuse;
    
    if( u_textures_count > 0 )
        surfaceColor = texture(u_texture, fragTexCoord);
        
    out_diffuse = surfaceColor;
	out_position = vec4(surfacePos,1);
	out_normals = vec4(normal,1);;
}