#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttribute(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLboolean normalized, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	//--------------------Link vertex attributes-------------------
	// (Tell Shaders How To Interpret The Vertex Data)
	glVertexAttribPointer(layout, numComponents, type, normalized, stride, offset); //weird cast just tells opengl that the start of the vertex data is at the start of the vertex buffer
	glEnableVertexAttribArray(layout); //the layout number used by the vertex shader

	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::UnBind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}