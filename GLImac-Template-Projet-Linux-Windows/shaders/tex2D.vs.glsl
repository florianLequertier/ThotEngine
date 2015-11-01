#version 330 core

layout(location = 0 ) in vec3 aVertexPosition;
layout(location = 1 ) in vec3 aVertexNormal;
layout(location = 2 ) in vec2 aTextureCoord;
layout(location = 3 ) in vec3 aPosition;
layout(location = 4 ) in float aIndiceTexture;
layout(location = 5 ) in float aHasFocus;


//uniform float uTime;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
out vec2 vTextureCoord;
out float vHasFocus;


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
    vHasFocus = aHasFocus;
    //vTextureCoord = aTextureCoord;

    vTextureCoord = vec2( aTextureCoord.x + aIndiceTexture, aTextureCoord.y);
    //mat3    mat_rot = rotate(uTime);


    mat4 MVP_matrix = uProjectionMatrix * uViewMatrix;
    vec4 modelVecteur = translate(aPosition) * vec4(aVertexPosition,1);
    vec4 pos =  MVP_matrix * modelVecteur;
    gl_Position = pos;

};

