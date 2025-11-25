//adapted from https://gamedev.stackexchange.com/questions/94061/rotating-an-object-with-quaternion
//and https://gameprogrammingpatterns.com/dirty-flag.html

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
	glm::mat4 Matrix;


	//constructor with default values - the quaternion default is a unit quaternion (no transformation)
	Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0)) : Matrix(glm::mat4(1.f))
	{
		Position = position;
		Scale = scale;
		Rotation = rotation;
		CalculateMatrix();
	};

	//use a Transform and glm::mat4 interchangably
	operator glm::mat4() const
	{
		return Matrix;
	}

	//allow explicit casting to a mat4
	explicit operator glm::mat4() const
	{
		return Matrix;
	}


	static Transform Origin(); //identity matrix representative of parent origin

	Transform Combine(Transform& other);

private:
	void CalculateMatrix();
};

