#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include"glm.hpp"
#include"gtc/type_ptr.hpp"


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

		void SetVec4(const char* name, glm::vec4) const;
		void SetVec3(const char* name, glm::vec3) const;
		void SetVec2(const char* name, glm::vec2) const;

		void SetMat4(const char* name, glm::mat4 value) const;
		void SetMat3(const char* name, glm::mat3 value) const;
		void SetMat2(const char* name, glm::mat2 value) const;
};

#endif