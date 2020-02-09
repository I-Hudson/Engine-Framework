#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out v2f
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;

layout(binding = 0) uniform UniformVertexObject
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ObjectMatrix;
    vec4 u_AmbiantLight;
    vec4 u_DirLight;
    vec4 u_ViewPos;
    float u_AmbiantInten;
}uvo;

void main() {
    gl_Position = uvo.u_Projection * inverse(uvo.u_View) * uvo.u_ObjectMatrix * vec4(inPosition.xyz, 1.0);
    
    o.Position = inPosition;
    o.Color = inColor;
    o.Normal = inNormal;
    o.UV = inUV;
}