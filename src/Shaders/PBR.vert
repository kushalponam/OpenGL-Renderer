#version 400 core
#pragma optionNV(unroll all)

#define MAXLIGHTS 2

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec3 TextureCoord;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat3 NormalMatrix;

struct Light{
	vec3 Light_Pos;
	mat4 LightMatrix;
	vec3 Light_Col;
};
uniform Light lights[MAXLIGHTS];

out LightOutData{
	vec3 LightDirection;
	vec3 HalfVector;
	vec3 LightPos_WorldSpace;
	vec4 FragPosLightSpace;
	vec3 Light_Col;
}fs_Out[MAXLIGHTS];

// V = Eye Dir, L = Light Dir, N = Normal , H = halfVector

out vec3 Normal;
out vec2 UV;
out vec3 FragPos;
out vec3 ViewDirection;

void CalculateLightData(vec3 vertexpos_cameraspace, vec3 EyeDirection ,vec3 fragPos){

	for(int i=0;i<MAXLIGHTS;i++){
		
		vec3 lightpos_cameraspace = (View * vec4(lights[i].Light_Pos,1)).xyz;
		fs_Out[i].LightDirection = (lightpos_cameraspace - vertexpos_cameraspace);

		fs_Out[i].HalfVector = normalize(fs_Out[i].LightDirection + EyeDirection);

		fs_Out[i].LightPos_WorldSpace = lights[i].Light_Pos;

		fs_Out[i].FragPosLightSpace = lights[i].LightMatrix * vec4(fragPos,1.0f);

		fs_Out[i].Light_Col = lights[i].Light_Col;
	
	}
}





void main()
{
    
	Normal = normalize(NormalMatrix * VertexNormal);
	
	vec3 vertexpos_cameraSpace = ( View * Model * vec4(VertexPosition,1)).xyz;
	vec3 EyeDirection = (vec3(0,0,0) - vertexpos_cameraSpace);

	gl_Position = MVP * vec4(VertexPosition,1.0f);
		
	FragPos = vec3( Model * vec4(VertexPosition,1) );

	CalculateLightData(vertexpos_cameraSpace,EyeDirection,FragPos);

	UV = vec2(TextureCoord);
	ViewDirection = normalize(EyeDirection);
}