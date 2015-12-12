#version 330 core

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_lightSpaceMatrix;

void main()
{
    vec4 vertexPosition = vec4(a_vertexPosition, 1);

    gl_Position = u_lightSpaceMatrix * u_ModelMatrix * vertexPosition;
};
