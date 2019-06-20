#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

typedef struct camera
{
	glm::vec3 position;

	float yaw;
	float pitch;

	glm::dvec2 cursor;
} camera;

glm::mat4 update(camera* camera);


typedef struct transform
{
	glm::vec3 translation;
	glm::quat rotation;
	float scale;
} transform;


#endif
