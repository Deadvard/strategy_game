#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

typedef struct camera
{
	glm::vec3 position;

	float yaw;
	float pitch;

	glm::dvec2 cursor;
} camera;

glm::mat4 update(camera* camera);


#endif
