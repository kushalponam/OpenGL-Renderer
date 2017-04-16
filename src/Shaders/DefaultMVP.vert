#version 400 core


layout(location=0) in vec3 VertexPosition;

uniform mat4 MVP;
out vec2 UV;

void main()
{
	gl_Position = MVP * vec4(VertexPosition,1.0);
	UV = (VertexPosition * 0.5 + 0.5).xy;

}