#version 330

layout(location = 0 ) in vec3 aVertexPosition;
layout(location = 1 ) in vec3 aNormal;
layout(location = 2 ) in vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
out vec2 vTextureCoord;

void main() {
    //vec4 WVP_Pos = uProjectionMatrix * uViewMatrix * vec4(aVertexPosition, 1.0);
    //gl_Position = WVP_Pos.xyww;
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix  *  vec4(aVertexPosition, 1.0);
    vTextureCoord = aTexCoord;

}
