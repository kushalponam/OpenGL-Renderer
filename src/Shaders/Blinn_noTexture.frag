#version 400 core
#pragma optionNV(unroll all)

#define MAXLIGHTS 2

layout (location = 0) out vec4 fColor;

uniform float SpecularStength;
uniform float Shininess;
uniform float AmbientIntensity;

uniform sampler2D ShadowMap[MAXLIGHTS];

in LightOutData{
	vec3 LightDirection;
	vec3 HalfVector;
	vec3 LightPos_WorldSpace;
	vec4 FragPosLightSpace;
	float LightIntensity;
}fs_in[MAXLIGHTS];


in vec3 Normal;
in vec2 UV;
in vec3 KD;
in vec3 FragPos;
in vec3 ViewDirection;

float ShadowCalculation(float ndotl,int light){
	
	vec3 projCoords = fs_in[light].FragPosLightSpace.xyz/fs_in[light].FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	projCoords = clamp(projCoords,0.0,1.0);

	float closestDepth = texture(ShadowMap[light],projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = 0;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
	
}


void main()
{
	vec3 rgb = vec3(0,0,0);
	float attenuation = 0;
	for(int i=0; i<MAXLIGHTS;i++){

		vec3 LDir = normalize(fs_in[i].LightDirection);

		float NdotL = max(0.0,dot(Normal,LDir));
		float NdotH = max(0.0,dot(Normal,fs_in[i].HalfVector));
	
	
		NdotH = pow(NdotH,Shininess) * SpecularStength;
	
		vec3 ambient = AmbientIntensity *KD;
		vec3 diffuse = fs_in[i].LightIntensity*KD;

		vec3 specular = vec3(1,1,1) * fs_in[i].LightIntensity * NdotH ;
	
		float shadow = ShadowCalculation(NdotL,i);
		float lightDistance = length(FragPos - fs_in[i].LightPos_WorldSpace);
		attenuation = 1.0f / (1.0f + (lightDistance * 0.09f) + (0.032f * lightDistance * lightDistance));
	
		ambient *= attenuation;
		diffuse *= attenuation;
		specular*= attenuation;

		rgb += ambient + (( (diffuse  ) + specular) * NdotL);
	
	}
	
	rgb /= MAXLIGHTS;

	float gamma = 2.2;
	rgb = pow(rgb,vec3(1.0/gamma));	
	
	fColor = vec4(rgb,1.0);
}