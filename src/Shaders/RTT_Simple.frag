#version 330 core

layout(location=0) out vec4 fColor;

in vec2 UV;

uniform sampler2D RTTTexture;

void main(){

	float depthValue = texture(RTTTexture,UV).r;
	fColor = vec4(depthValue,depthValue,depthValue,1.0f);
	
}