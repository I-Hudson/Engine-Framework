#type vertex
#version 330

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec2 inUV;

uniform mat4 u_ProjectionView;
uniform mat4 u_ObjectMatrix;

out v2f
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;

int main()
{
	o.Position = inPosition;
	o.Color = inColor;
	o.Normal = inNormal;
	o.UV = inUV;

	gl_Position = u_ProjectionView * u_ObjectMatrix * inPosition;
}

#type fragment
#version 330

layout(location = 0) out vec4 outColor;

in v2f
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;

int main()
{
	outColor = o.Color;
}

