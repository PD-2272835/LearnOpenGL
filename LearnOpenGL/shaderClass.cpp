#include"shaderClass.h"

//read shader file source from a given filename
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}



Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
	std::string vertexCode = get_file_contents(vertexFilePath);
	std::string fragmentCode = get_file_contents(fragmentFilePath);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	//----------------------------Vertex Shader----------------------------
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //create vertex shader
	glShaderSource(vertexShader, 1, &vertexSource, NULL); //assign vertexShader source code to vertexShaderSource
	glCompileShader(vertexShader); //compile the vertex shader to machine code

	//------------------------Fragment Shader------------------------------
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create fragment shader
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL); //assign fragmentShader source code to fragmentShaderSource
	glCompileShader(fragmentShader); //compile fragment shader



	//------------Shader Program (linking all shaders together)------------
	ID = glCreateProgram();

	//create the shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);


	//shaderProgram compilation check
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Shader Program Failed To Compile\n" << infoLog << std::endl;
	} else {
		std::cout << "Shader Compilation Success\n" << std::endl;
	}

	//clean up the shader objects as we no longer require them in memory after full program compilation
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}



void Shader::Activate()
{
	glUseProgram(ID);
}


void Shader::Delete()
{
	glDeleteProgram(ID);
}



//search the compiled shader held by this class for a uniform called uniformName
GLint Shader::GetUniformLocation(const char* uniformName) const
{
	return glGetUniformLocation(ID, uniformName);
}


//--------------setting uniform values by name--------------
void Shader::SetBool(const char* name, bool value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const char* name, GLint value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const char* name, GLfloat value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

//vector support
void Shader::SetVec4(const char* name, glm::vec4 value) const
{
	glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const char* name, glm::vec3 value) const
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec2(const char* name, glm::vec2 value) const
{
	glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

//matrix support
void Shader::SetMat4(const char* name, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(const char* name, glm::mat3 value) const
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat2(const char* name, glm::mat2 value) const
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}
