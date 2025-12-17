#include"VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID); //create a buffer object and store its ID
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//VBO::Bind(); //maybe? : no
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); //copy data from vertices into the buffer
	
	/* how the GPU should manage the data in the buffer :
	GL_STATIC - Data is set once and used many times
	GL_STREAM - Data is set only once and used only a couple of times at most
	GL_DYNAMIC - data is changed and used many times */
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID); //tell openGL this is a vertex buffer, and that it is the buffer we are working with
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
	this->Unbind();
}