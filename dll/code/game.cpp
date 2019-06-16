#include "game.h"
#include <stdio.h>

#include "memory.h"

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
	
}