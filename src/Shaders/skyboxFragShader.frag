#version 330 core

layout (location = 0) out vec4 fColor;

uniform samplerCube cubeMap;
in vec3 texCoords;

void main() {

	fColor = vec4(texture(cubeMap,texCoords).rgb,1.0);
	
}