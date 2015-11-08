#version 330 core

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_color;

uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform mat4 u_normalMatrix;

out vec3 v_color;
out vec2 v_texCoords;
out vec3 v_position_vs;
out vec3 v_normal_vs;

void main()
{
	//coordonnées homogénes
	vec4 vertexPosition = vec4(a_vertexPosition, 1);
	vec4 vertexNormal = vec4(a_vertexNormal, 0);

	v_position_vs = vec3(u_MVMatrix * vertexPosition);
	v_normal_vs = vec3(u_normalMatrix * vertexNormal);
	v_color = a_color;
	v_texCoords = a_texCoords;

    gl_Position = u_MVPMatrix * vertexPosition;
};
