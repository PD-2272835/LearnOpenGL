#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
	private:
		GLint GetUniformLocation(const char* uniformName) const;

	public:
		GLuint ID;
		Shader(const char* vertexFilePath, const char* fragmentFilePath);

		void Activate();
		void Delete();

		void SetBool(const char* name, bool value) const;
		void SetInt(const char* name, GLint value) const;
		void SetFloat(const char* name, GLfloat value) const;
};

#endif