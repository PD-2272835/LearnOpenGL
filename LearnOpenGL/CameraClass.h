#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float MOVEMENTSPEED = 2.5f;
const float MOUSESENSITIVITY = 0.1f;


enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	//camera space definitions
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp; //world space up

	//euler angles (degrees)
	float Yaw;
	float Pitch;

	//camera settings
	float MovementSpeed;
	float MouseSensitivity;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(MOVEMENTSPEED), MouseSensitivity(MOUSESENSITIVITY)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(double xOffset, double yOffset);
	//void ProcessMouseScroll(float yOffset);



private:
	void UpdateCameraVectors();

};