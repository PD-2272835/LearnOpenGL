#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
	public:
		GLuint ID;
		VAO();

		void LinkAttribute(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLboolean normalized, GLsizeiptr stride, void* offset);
		void Bind();
		void UnBind();
		void Delete();
};

#endif