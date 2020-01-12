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

void main()
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

uniform vec3 u_DirLight0;
uniform vec3 u_ViewPos;

uniform sampler2D diffuseTexture;
uniform sampler2D heightTexture;

in v2f
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;


void main()
{
	vec4 diffTex = texture(diffuseTexture, o.UV);
	vec4 normTex = o.Normal;//texture(heightTexture, o.UV);

	vec3 ambiant = 0.05 * vec3(0.15f, 0.15f, 0.15f);

	vec3 lightDir = normalize(u_DirLight0 - o.Position.xyz);
	float diff = max(dot(lightDir, normTex.xyz), 0.0);
	vec3 diffuse = diff * diffTex.xyz;

	vec3 viewDir = normalize(u_ViewPos - o.Position.xyz);
	vec3 reflectDir = reflect(-lightDir, normTex.xyz);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normTex.xyz, reflectDir), 0.0), 8.0);
	vec3 specular = vec3(0.3) * spec;

	
	outColor = vec4(ambiant + diffuse + specular, 1.0);
}

