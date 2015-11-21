#version 330

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_color;

uniform mat4 u_viewMat;
uniform mat4 u_projectionMat;

out vec3 v_texCoords;

void main() {

    vec4 pos = u_projectionMat * u_viewMat * vec4(a_vertexPosition, 1.0);
    gl_Position = pos.xyww;
    v_texCoords = a_vertexPosition;

}
