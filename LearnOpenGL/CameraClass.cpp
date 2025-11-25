#include "CameraClass.h"
#include <iostream>

#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}


void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;

	if (direction == FORWARD) {
		Position += Front * velocity;
	}
	if (direction == BACKWARD) {
		Position -= Front * velocity;
	}
	if (direction == LEFT) {
		Position -= Right * velocity;
	}
	if (direction == RIGHT) {
		Position += Right * velocity;
	}
}



void Camera::ProcessMouseMovement(double xOffset, double yOffset)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (Pitch >= 89.0f) { Pitch = 89.0f; }
	if (Pitch <= -89.0f) { Pitch = -89.0f; }
	Camera::UpdateCameraVectors();
}



void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp)); //normalized to prevent getting smaller + slowing movement as pitch increases
	Up = glm::normalize(glm::cross(Right, Front));
}


#endif