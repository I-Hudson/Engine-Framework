#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformVertexObject
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ObjectMatrix;
    vec3 u_testColour;
}uvo;

void main() {
    gl_Position = uvo.u_Projection * uvo.u_View * uvo.u_ObjectMatrix * vec4(inPosition.xyz, 1.0);
    fragColor = vec3(1.0, 1.0, 1.0);
}