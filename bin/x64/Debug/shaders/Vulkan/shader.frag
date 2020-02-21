#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in v2f
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec2 UV;
}o;

layout(location = 0) out vec4 outColor;

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

void main() 
{
    vec4 diffTex = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 normTex = o.Normal;//texture(heightTexture, o.UV);
	
	vec3 ambiant = uvo.u_AmbiantLight.xyz * uvo.u_AmbiantInten;
	
	vec3 lightDir = normalize(uvo.u_DirLight.xyz - o.Position.xyz);
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
	
	vec3 viewDir = normalize(uvo.u_ViewPos.xyz - o.Position.xyz);
	vec3 reflectDir = reflect(-lightDir, normTex.xyz);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normTex.xyz, reflectDir), 0.0), 8.0);
	vec3 specular = vec3(0.3) * spec;
	
	outColor = vec4(ambiant + diffuse, 1.0);


    outColor = vec4(outColor.xyz, 1.0);
}