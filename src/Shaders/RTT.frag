#version 330 core

layout (location = 0) out vec4 fColor;



uniform float LightIntensity;
uniform float SpecularStength;
uniform float Shininess;
uniform float AmbientIntensity;

uniform sampler2DRect RTTTexture;

in vec3 Normal;
in vec3 EyeDir;
in mat4 view;
in vec3 LightDirection;
in vec3 HalfVector;
in vec3 KD;
in vec2 RTTUVCoord;
in vec4 ClipSpace;

void main() {
	vec3 LDir = normalize(LightDirection);

	float NdotL = max(0.0,dot(Normal,LDir));
	float NdotH = max(0.0,dot(Normal,HalfVector));
	
	NdotH = pow(NdotH,Shininess) * SpecularStength;
	
	vec3 ambient = AmbientIntensity *KD;
	vec3 diffuse = LightIntensity*KD;

	vec3 eyeDir = normalize(EyeDir);
	float vDotN = dot(eyeDir,Normal);
	
	vec4 RTTCol = texture(RTTTexture,gl_FragCoord.xy) * 0.8;
	
	vec3 specular = vec3(1,1,1) * LightIntensity * NdotH ;
		
	vec3 blinnCol = ambient +(( (diffuse * RTTCol.rgb ) + specular) * NdotL) ;


	fColor = vec4(blinnCol,1.0);
	
}