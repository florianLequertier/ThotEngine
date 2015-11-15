#version 330

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_color;

uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform mat4 u_normalMatrix;

out vec2 v_texCoords;

void main() {

    gl_Position = u_MVPMatrix  *  vec4(a_vertexPosition, 1.0);
    v_texCoords = a_texCoords;

}
