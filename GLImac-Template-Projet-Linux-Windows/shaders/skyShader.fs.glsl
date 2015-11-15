#version 330

in vec2 v_texCoords;
out vec4 fragColor;

uniform sampler2D u_diffuse;

void main (void) {

    fragColor = vec4( texture(u_diffuse, v_texCoords).xyz, 1 );
}
