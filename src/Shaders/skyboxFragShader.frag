#version 330 core

layout (location = 0) out vec4 fColor;

uniform samplerCube cubeMap;
uniform float roughness;
in vec3 texCoords;

void main() {

	fColor = vec4(textureLod(cubeMap,texCoords,roughness).rgb,1.0);
	
}