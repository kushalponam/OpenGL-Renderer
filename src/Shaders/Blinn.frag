#version 330 core

layout (location = 0) out vec4 fColor;

uniform float LightIntensity;
uniform float SpecularStength;
uniform float Shininess;
uniform float AmbientIntensity;


uniform sampler2D diffTextureSampler;
uniform sampler2D specTextureSampler;


in vec3 Normal;
in vec3 LightDirection;
in vec3 HalfVector;
in vec2 UV;
in vec3 KD;

void main()
{
	vec3 LDir = normalize(LightDirection);

	float NdotL = max(0.0,dot(Normal,LDir));
	float NdotH = max(0.0,dot(Normal,HalfVector));
	
	
	NdotH = pow(NdotH,Shininess) * SpecularStength;
	
	vec3 ambient = AmbientIntensity *KD;
	vec3 diffuse = LightIntensity*KD;
	
	vec3 specCol = texture2D(specTextureSampler,UV).rgb;

	vec3 specular = specCol * LightIntensity * NdotH ;
	
	vec3 texcol = texture2D(diffTextureSampler, UV).rgb;
	
	vec3 rgb = ambient +(( (diffuse* texcol) + specular) * NdotL);

	fColor = vec4(rgb,1.0);
}