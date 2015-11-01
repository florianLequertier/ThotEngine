#version 330 core

layout(location = 0 ) in vec2 aVertexPosition;
layout(location = 1 ) in vec2 aTextureCoord;


uniform mat3 uModelMatrix;

out vec2 vTextureCoord;



void main()
{
    vTextureCoord = vec2( aTextureCoord.x , aTextureCoord.y);

    vec3 modelVecteur = vec3(aVertexPosition,1);
    vec4 pos =  vec4( vec2(uModelMatrix * modelVecteur).xy, 0, 1);
    gl_Position = pos;

};
