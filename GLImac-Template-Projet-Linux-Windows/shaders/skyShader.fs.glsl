#version 330

in vec3 v_texCoords;
out vec4 fragColor;

uniform samplerCube u_diffuse;

void main (void) {

    fragColor = vec4(texture(u_diffuse, v_texCoords).rgb, 1);
}
