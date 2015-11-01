#version 330 core


out vec3 fFragColor;
in vec2 vTextureCoord;
uniform sampler2D uTexture;


void main()
{
    fFragColor = vec3(1,0,0);//texture(uTexture, vTextureCoord).xyz;
};

