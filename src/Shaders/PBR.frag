#version 400 core
#pragma optionNV(unroll all)

#define MAXLIGHTS 2

layout (location = 0) out vec4 fColor;

in LightOutData{
	vec3 LightDirection;
	vec3 HalfVector;
	vec3 LightPos_WorldSpace;
	vec4 FragPosLightSpace;
	vec3 Light_Col;
}fs_in[MAXLIGHTS];

uniform sampler2D ShadowMap[MAXLIGHTS];
uniform samplerCube irradianceMap;

in vec3 Normal;
in vec2 UV;
in vec3 FragPos;
in vec3 ViewDirection;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
//uniform float ao;

const float PI = 3.14159;


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


float DistributionGGX(float NdotH, float roughness)
{
	float a = roughness * roughness;
	float a2 = a*a;

	float NdotH2 = NdotH*NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);

	denom = PI * denom * denom;

	return nom/denom;

}

float GeometrySchlickGGX(float NdotV, float roughness){
	float r = (roughness + 1.0);
	float k = (r*r)/8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom/denom;
}

float GeometrySmith(float NdotV, float NdotL, float roughness){

	float ggx2 = GeometrySchlickGGX(NdotV,roughness);
	float ggx1 = GeometrySchlickGGX(NdotL,roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchilick(float cosTheta, vec3 F0,float roughness){

	return F0 + (max(vec3(1.0-roughness),F0) - F0) * pow (1.0-cosTheta,5.0);
}

void main()
{
	vec3 F0 = vec3(0.04);
	F0 = mix(F0,albedo,metallic);

	vec3 Lo = vec3(0.0);
	float NdotV = max(dot(Normal,ViewDirection),0.0);

	vec3 finalKD= vec3(0.0);
	for (int i=0 ; i< MAXLIGHTS;i++){
		vec3 Ldir = normalize(fs_in[i].LightDirection);
		float NdotL = max(0.0,dot(Normal, Ldir));
		float NdotH = max(0.0,dot(Normal,fs_in[i].HalfVector));
		
		float distance = length(fs_in[i].LightPos_WorldSpace - FragPos);
		float attenuation =  1.0f / (1.0f + (distance * 0.09f) + (0.032f * distance * distance));

		vec3 radiance = fs_in[i].Light_Col * attenuation;

		float NDF = DistributionGGX(NdotH, roughness);
		float G = GeometrySmith(NdotV,NdotL,roughness);
		
		
		vec3 F = fresnelSchilick(NdotV,F0,roughness);
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		finalKD += kD;

		vec3 nominator = NDF * G * F;
		float denominator = 4 * NdotV * NdotL + 0.001;
		vec3 brdf = nominator / denominator;

		float shadow = ShadowCalculation(NdotL,i);

		Lo += ( kD * albedo*(1.0-shadow) / PI + brdf )* radiance * NdotL;
	}

	vec3 irradiance = texture(irradianceMap,Normal).rgb;
	
	finalKD = finalKD/(finalKD  + vec3(1.0));
	
	vec3 diffuse = irradiance * albedo;
	vec3 ambient = finalKD * diffuse ;

	vec3 color = ambient + Lo;

	color = color/(color + vec3(1.0));
	color = pow(color , vec3(1.0/2.2));

	fColor = vec4( color  ,1.0);

}