#pragma once
#include "VAO.h";
#include "EBO.h";
#include "shaderClass.h"

class Mesh
{
public:
	VAO* Vertices;
	EBO* Elements;
	Shader* ShaderProgram;

	Mesh(VAO& vertices, EBO& elements, Shader& shaderProgram)
	{
		Vertices = &vertices;
		Elements = &elements;
		ShaderProgram = &shaderProgram;
	};

	void Render(glm::mat4& transform);
};

