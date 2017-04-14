#version 330 core

layout(location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat3 NormalMatrix;
uniform vec3 Light_Pos;
uniform vec3 kd;


out vec3 EyeDir;
out vec3 Normal;
out vec3 LightDirection;
out vec3 HalfVector;
out vec3 KD;
out mat4 view;
out vec2 RTTUVCoord;
out vec4 ClipSpace;

void main(){

	Normal = normalize(NormalMatrix * VertexNormal);

	vec3 pos_camSpace = ( View * Model * vec4(VertexPosition,1)).xyz;
	EyeDir = vec3(0,0,0) - pos_camSpace;
	
	vec3 LightPos = (View  * vec4(Light_Pos,1)).xyz;
	LightDirection = (LightPos - pos_camSpace);

	HalfVector = normalize(LightDirection + EyeDir);

	ClipSpace = MVP * vec4(VertexPosition,1.0f);
	gl_Position = ClipSpace;
	
	view = View;
	KD = kd;

	RTTUVCoord = (VertexPosition.xy + vec2(0.5,0.5))/1.0;

}

