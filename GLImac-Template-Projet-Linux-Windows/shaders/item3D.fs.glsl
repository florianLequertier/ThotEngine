#version 330 core


out vec4 fFragColor;
uniform sampler2D uTexture;
in vec2 vTextureCoord;


void main()
{

    fFragColor = texture(uTexture, vTextureCoord).xyzw;

};

