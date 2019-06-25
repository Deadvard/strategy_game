#ifndef GAME_H
#define GAME_H

#include "camera.h"

typedef struct game_memory
{
	unsigned int storage_size;
	unsigned char* storage;
} game_memory;

typedef struct game_state
{
	unsigned int is_initialized;
	camera camera;
	glm::mat4 view;
} game_state;



typedef struct game_button
{
	unsigned down;
	unsigned transitions;
} game_button;

typedef struct game_input
{
	double delta_time;
	
	int mouse_x;
	int mouse_y;

	game_button select;
	game_button cancel;

	game_button up;
	game_button left;
	game_button down;
	game_button right;

} game_input;



extern "C" void tick(game_memory* memory, game_input* input);

#endif
