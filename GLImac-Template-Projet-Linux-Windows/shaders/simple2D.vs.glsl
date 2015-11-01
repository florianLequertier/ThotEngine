#version 330 core

layout(location = 0 ) in vec2 aVertexPosition;


uniform mat3 uModelMatrix;



void main()
{

    vec3 modelVecteur = vec3(aVertexPosition,1);
    vec4 pos =  vec4( vec2(uModelMatrix * modelVecteur).xy, 0, 1);
    gl_Position = pos;

};
