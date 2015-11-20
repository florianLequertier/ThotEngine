#version 330 core
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_color;

out vec2 v_texCoords;

void main()
{
    gl_Position = vec4(a_vertexPosition, 1.0f);
    v_texCoords = a_texCoords;
}
