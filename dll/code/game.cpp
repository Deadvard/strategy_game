#include "game.h"
#include <stdio.h>

#include "memory.h"
#include "level.h"

void tick(game_memory* memory, game_input* input)
{
	game_state* state = (game_state*)memory->storage;
	
	if (!input->up.down && input->up.transitions)
	{
		printf("up\n");
	}

	if (input->down.down)
	{
		printf("down\n");
	}

	glm::mat4 mat = update(&state->camera);
	glm::vec3 forward = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
	glm::vec3 right = glm::vec3(mat[0][0], mat[1][0], mat[2][0]);

	float speed = 10.0f * (float)input->delta_time;
	if (input->up.down) state->camera.position -= speed * forward;
	if (input->left.down) state->camera.position -= speed * right;
	if (input->down.down) state->camera.position += speed * forward;
	if (input->right.down) state->camera.position += speed * right;

	state->view = mat;
	
}

typedef struct mesh
{

} mesh;