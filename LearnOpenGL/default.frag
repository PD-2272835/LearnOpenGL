#version 330 core
out vec4 FragColor;

in vec3 VertexColor;

uniform float alphaModulator;

void main()
{
	FragColor = vec4(VertexColor, 1.0f);
}
