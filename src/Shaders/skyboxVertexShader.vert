#version 330 core

layout(location=0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
out vec3 texCoords;

void main(){

	gl_Position = projectionMatrix * viewMatrix * vec4(position,1.0f);
	texCoords = position;

}