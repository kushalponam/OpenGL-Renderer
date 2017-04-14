#version 330 core

layout(location=0) in vec3 vertexPosition;

uniform mat4 MVP;
out vec2 UV;

void main() {

	gl_Position = vec4(vertexPosition ,1);
	UV = (vertexPosition.xy + vec2(1.0,1.0))/2.0;
}