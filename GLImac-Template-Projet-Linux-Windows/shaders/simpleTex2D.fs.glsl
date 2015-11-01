#version 330 core


out vec4 fFragColor;
in vec2 vTextureCoord;
uniform sampler2D uTexture;


void main()
{
    fFragColor = texture(uTexture, vTextureCoord).xyzw;
};

