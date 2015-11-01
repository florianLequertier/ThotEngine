#version 330 core

layout(location = 0 ) in vec3 aVertexPosition;
layout(location = 1 ) in vec3 aVertexNormal;
layout(location = 2 ) in vec2 aTextureCoord;


//uniform float uTime;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uMVP;
uniform mat4 uNormalMatrix;

out vec3 vPosition_vs;
out vec3 vNormal;
out vec2 vTextureCoord;



mat3 rotate(float a)
{
    a= radians(a);
    return mat3(vec3(cos(a),sin(a),0),vec3(-sin(a),cos(a),0),vec3(0,0,1));
}

mat4 translate(vec3 dep )
{
    return mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(dep.x,dep.y,dep.z,1));
}



void main()
{
    vTextureCoord = aTextureCoord;


    vNormal = aVertexNormal;// vec3( uNormalMatrix * vec4(aVertexNormal,0) );
    vec4 temp_vPosition_vs = /*uViewMatrix */ uModelMatrix * vec4(aVertexPosition, 1);
    vPosition_vs = vec3 ( temp_vPosition_vs ); //position non projetée.


    vec4 pos =  uMVP * vec4(aVertexPosition,1);
    gl_Position = pos;


};
