#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Transform.h"


Transform Transform::Origin()
{
	return Transform();
}

Transform Transform::Combine(Transform& other)
{
	//return other.Matrix * Matrix; this should not be permitted as it is directly modifying the transformation matrix held within this object without recalulating the vectors used
	return Transform();
}

void Transform::CalculateMatrix()
{
	glm::mat4 translate = glm::translate((glm::mat4)Transform::Origin(), Position);
	glm::mat4 rotate = glm::mat4_cast(Rotation);
	glm::mat4 scale = glm::scale((glm::mat4)Transform::Origin(), Scale);

	Matrix = translate * rotate * scale;
}

#endif