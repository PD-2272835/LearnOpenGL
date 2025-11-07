#version 420 core
layout (location = 0) in vec3 aPos;

out vec4 VertexColor;
void main()
{
   gl_Position = vec4(aPos, 1.0); //can pass vectors into other vectors constructors
   VertexColor = vec4(aPos, 1.0);
}