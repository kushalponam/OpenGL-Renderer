#version 330 core

layout (location = 0) out vec4 fColor;



uniform float LightIntensity;
uniform float SpecularStength;
uniform float Shininess;
uniform float AmbientIntensity;

uniform samplerCube cubeMap;


in vec3 Normal;
in vec3 EyeDir;
in mat4 view;
in vec3 LightDirection;
in vec3 HalfVector;
in vec3 KD;

void main() {
	vec3 LDir = normalize(LightDirection);

	float NdotL = max(0.0,dot(Normal,LDir));
	float NdotH = max(0.0,dot(Normal,HalfVector));
	
	NdotH = pow(NdotH,Shininess) * SpecularStength;
	
	vec3 ambient = AmbientIntensity *KD;
	vec3 diffuse = LightIntensity*KD;


	vec3 eyeDir = normalize(EyeDir);
	float vDotN = dot(eyeDir,Normal);
	vec3 reflected = 2 * Normal * vDotN - eyeDir;
	reflected = vec3( inverse(view) * vec4(reflected,0.0));
	
	vec3 refColor = texture(cubeMap,reflected).rgb;

	vec3 specular = vec3(1,1,1) * LightIntensity * NdotH ;

	vec3 rgb = ambient +(( (diffuse * refColor) + specular) * NdotL);


	fColor = vec4(rgb  ,1.0);

}