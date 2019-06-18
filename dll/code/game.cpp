#include "game.h"
#include <stdio.h>

#include "memory.h"
#include "camera.h"

void tick(game_memory* memory, game_input* input)
{
	if (!input->up.down && input->up.transitions)
	{
		printf("up\n");
	}

	if (input->down.down)
	{
		printf("down\n");
	}

	camera camera;
	glm::mat4 mat = update(&camera);
	glm::vec3 forward = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
	glm::vec3 right = glm::vec3(mat[0][0], mat[1][0], mat[2][0]);

	glm::vec3 oldPos = camera.position;

	float speed = 10.0f * (float)input->delta_time;
	if (input->up.down) camera.position -= speed * forward;
	if (input->left.down) camera.position -= speed * right;
	if (input->down.down) camera.position += speed * forward;
	if (input->right.down) camera.position += speed * right;
	
}