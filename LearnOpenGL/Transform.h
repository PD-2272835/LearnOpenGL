//adapted from https://gamedev.stackexchange.com/questions/94061/rotating-an-object-with-quaternion

#pragma once

//Math Libraries
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Transform
{
public:
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::quat Rotation; 


	//constructor with default values
	Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::quat rotation = glm::quat(sqrt(1.f / 2), 0, sqrt(1.f / 2), 0))
	{
		Position = position;
		Scale = scale;
		Rotation = rotation;
	};

	//use a Transform and glm::mat4 interchangably
	operator glm::mat4() const
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), Position);
		glm::mat4 rotate = glm::mat4_cast(Rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), Scale);

		return translate * rotate * scale;
	};


	static glm::mat4 Origin();//identity matrix representative of parent origin

	//Transform Combine(Transform& other);
};

