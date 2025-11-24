#include"EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID); //create a buffer object and store it's ID
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); //copy data from indices into the buffer
	Size = size;
	/* how the GPU should manage the data in the buffer :
	GL_STATIC - Data is set once and used many times
	GL_STREAM - Data is set only once and used only a couple of times at most
	GL_DYNAMIC - data is changed and used many times */
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); //tell openGL this is an element (index) buffer, and that it is the buffer we are working with
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}