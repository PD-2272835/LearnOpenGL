#version 420 core
layout (location = 0) in vec3 aPos;		//vertex position is the first vec3 attribute
layout (location = 1) in vec3 aColor;

out vec4 VertexColor; //output to fragment shader

void main()
{
   gl_Position = vec4(aPos, 1.0); //can pass vectors into other vectors constructors
   VertexColor = vec4(aColor, 1.0);
}