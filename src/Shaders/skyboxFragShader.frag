#version 330 core

layout (location = 0) out vec4 fColor;

uniform samplerCube cubeMap;
in vec3 texCoords;

void main() {

	fColor = vec4(textureLod(cubeMap,texCoords,0.0).rgb,1.0);
	
}