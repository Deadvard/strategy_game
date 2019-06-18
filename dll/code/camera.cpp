#include "camera.h"

#include <glm/gtc/quaternion.hpp>
#define pi32 3.14159265359f

glm::mat4 update(camera* camera)
{
	float sensitivity = 0.01f;
	float max = pi32 * 0.4f;
	float min = -max;

	camera->yaw += (float)camera->cursor.x * sensitivity;
	camera->pitch += (float)camera->cursor.y * sensitivity;
	if (camera->pitch < min) camera->pitch = min;
	if (camera->pitch > max) camera->pitch = max;

	glm::quat qPitch = glm::angleAxis(camera->pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(camera->yaw, glm::vec3(0, 1, 0));

	glm::quat qRotate = glm::normalize(qPitch * qYaw);
	glm::mat4 rotate = glm::mat4_cast(qRotate);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -camera->position);

	return rotate * translate;
}