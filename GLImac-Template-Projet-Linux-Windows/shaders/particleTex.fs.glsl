#version 330 core

// Interpolated values from the vertex shaders
in vec4 particlecolor;
in vec2 vTexCoord;

// Ouput data
out vec4 color;
uniform sampler2D uTexture;


void main(){

        color =  vec4(texture(uTexture, vTexCoord).xyz, texture(uTexture, vTexCoord).w*particlecolor.w);//particlecolor; //vec4(1.f,0.f,0.f,1.f);//

}
