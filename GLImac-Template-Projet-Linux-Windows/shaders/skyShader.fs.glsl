#version 330

in vec2 vTextureCoord;
out vec4 fragColor;
//uniform samplerCube cubemap_texture;
uniform sampler2D cubemapTexture;

void main (void) {

    fragColor = vec4( texture(cubemapTexture, vTextureCoord).xyz, 1 );
}
