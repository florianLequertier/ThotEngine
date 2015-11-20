#version 330 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gDiffuse;
layout(location = 3) out vec3 gSpecShininess;

in vec3 v_color;
in vec2 v_texCoords;
in vec3 v_normals;

in vec3 v_fragPos;

uniform sampler2D u_diffuse;

uniform float u_specularValue;
uniform float u_shininessValue;

void main()
{
    gPosition = v_fragPos;
    gNormal = v_normals;
    gDiffuse = texture(u_diffuse, v_texCoords).rgb * v_color;
    gSpecShininess.r = u_specularValue;
    gSpecShininess.g = u_shininessValue;
};
