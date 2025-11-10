#version 420 core
out vec4 FragColor;

in vec3 VertexColor;

uniform float alphaModulator; //uniform syntax, passing global values into fragment shader

void main()
{
	FragColor = vec4(VertexColor, alphaModulator);
}
