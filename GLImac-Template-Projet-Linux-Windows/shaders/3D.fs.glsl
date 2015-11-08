#version 330 core

in vec3 v_color;
in vec2 v_texCoords;
 
out vec4 fFragColor;

//uniform sampler2D u_diffuse;

void main()
{
	//vec4 texColor = texture(u_diffuse, v_texCoords);
    fFragColor = vec4(255, 0, 0, 255);//vec4(v_color * vec3(texColor), texColor.a);
};
