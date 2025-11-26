#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Transform.h"


glm::mat4 Transform::Origin()
{
	return glm::mat4(1.f);
}

glm::mat4 Transform::Combine(Transform& other)
{
	//return other.Matrix * Matrix; this should not be permitted as it is directly modifying the transformation matrix held within this object without recalulating the vectors used
	/*Position += other.Position;
	Rotation *= other.Rotation;
	Scale += other.Scale;
	CalculateMatrix();*/
	return Matrix * other.Matrix;
}

void Transform::CalculateMatrix()
{
	glm::mat4 translate = glm::translate(Transform::Origin(), Position);
	glm::mat4 rotate = glm::mat4_cast(Rotation);
	glm::mat4 scale = glm::scale(Transform::Origin(), Scale);

	Matrix = translate * rotate * scale;
}

#endif