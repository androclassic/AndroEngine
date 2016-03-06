//#version 330
//out vec4 fragColor;
//
//in vec4 fragmentColor;
//void main(){
//
//    fragColor = fragmentColor;
//}

#version 330

uniform mat4 model;

// material settings
uniform sampler2D u_texture;
uniform float u_shininess;
uniform vec4 u_color_specular;
uniform vec4 u_color_diffuse;

uniform struct Light {
    vec3 position;
    vec3 intensities; //a.k.a the color of the light
    float attenuation;
    float ambientCoefficient;
} light;

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragColor;
in vec3 fragVert;
in vec3 cameraPosition;

out vec4 finalColor;

void main() {

    finalColor = vec4(fragColor, 1.0);
    //finalColor = vec4(1.0f);
}
