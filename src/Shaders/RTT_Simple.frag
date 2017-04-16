#version 330 core

layout(location=0) out vec4 fColor;

in vec2 UV;

uniform sampler2D RTTTexture;

void main(){

	vec2 depthValue = texture2D(RTTTexture,UV).rg;
	fColor = vec4(depthValue,0.0f,1.0f);
}