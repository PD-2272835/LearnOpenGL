#version 420 core
out vec4 FragColor;

in vec4 VertexColor;

uniform vec4 ProgColor;

void main()
{
	FragColor = vec4(ProgColor);
}
