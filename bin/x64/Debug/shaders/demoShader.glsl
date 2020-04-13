#type vertex
#version 330

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec2 inUV;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_ObjectMatrix;

out v2f
{
	vec4 Position;
	vec4 WorldPosition;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;

void main()
{
	o.Position = inPosition;
	o.WorldPosition = u_ObjectMatrix * inPosition;
	o.Color = inColor;
	o.Normal = inNormal;
	o.UV = inUV;

	gl_Position = u_Projection * inverse(u_View) * u_ObjectMatrix * inPosition;
}

#type fragment
#version 330

layout(location = 0) out vec4 Diffuse;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 WorldPosition;

uniform vec4 u_AmbiantLight;
uniform float u_AmbiantLightInten;

uniform vec4 u_DirLight;
uniform vec4 u_ViewPos;

uniform sampler2D specular;
uniform sampler2D diffuse;

in v2f
{
	vec4 Position;
	vec4 WorldPosition;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;


void main()
{
	vec4 diffTex = texture(diffuse, o.UV);
	vec4 normTex = o.Normal;//texture(heightTexture, o.UV);

	vec3 ambiant = u_AmbiantLightInten * u_AmbiantLight.xyz;

	vec3 lightDir = normalize(u_DirLight.xyz - o.Position.xyz);
	float diff = max(dot(lightDir, normTex.xyz), 0.0);

	if (diff < 0.3)
	{
		diff = 0.3;
	}
	else if (diff < 0.6)
	{
		diff = 0.6;
	}
	else
	{
		diff = 1.0;
	}

	vec3 diffuse = diff * diffTex.xyz;

	vec3 viewDir = normalize(u_ViewPos.xyz - o.Position.xyz);
	vec3 reflectDir = reflect(-lightDir, normTex.xyz);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normTex.xyz, reflectDir), 0.0), 8.0);
	vec3 specular = vec3(0.3) * spec;
	
	Diffuse = vec4(ambiant + diffuse + specular, 1.0);
	Normal = normTex;
	WorldPosition = o.WorldPosition;
}

