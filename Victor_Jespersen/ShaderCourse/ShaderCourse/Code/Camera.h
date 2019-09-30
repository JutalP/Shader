#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
public:
	glm::vec3 Position = glm::vec3(0.f);
	float Yaw = 0.f;
	float Pitch = 0.f;

	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	void AddYaw(float YawAngle);
	void AddPitch(float PitchAngle);
	glm::mat4 GetViewMatrix();
};