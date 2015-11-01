#version 330 core

//in vec3 vFragColor;

uniform vec3 uColor;
//uniform sampler2D uTexture;
out vec3 fFragColor;
in vec2 vTextureCoord;
uniform sampler2D uTexture;
in float vHasFocus;


void main()
{
    if(vHasFocus<0.5f)
    fFragColor = texture(uTexture, vTextureCoord).xyz; // texture(uTexture, vTextureCoord).rgb;
    else
    fFragColor = /*texture(uTexture, vTextureCoord).xyz/2.f+*/vec3(0,1,0);

};

