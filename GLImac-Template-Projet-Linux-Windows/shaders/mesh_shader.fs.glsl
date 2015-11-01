#version 330 core

out vec4 fFragColor;
in vec2 vTextureCoord;


uniform sampler2D material_texture_diffuse1;

void main()
{
    //fFragColor = vec4(vTextureCoord, 0,1);
    fFragColor = texture(material_texture_diffuse1, vTextureCoord).xyzw; // texture(uTexture, vTextureCoord).rgb;

};
