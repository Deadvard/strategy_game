#include "game.h"
#include "window.h"
#include "game_stack.h"
#include "graphics.h"

#include <stdio.h>
#include <memory.h>

static void update(game_data* gd);

void run()
{
	initialize_window();

	game_data* gd = new game_data;
	memset(gd, 0, sizeof(game_data));
	gd->x = 10;
	gd->y = 10;
	
	graphics g = create_graphics();
	

	double last_time = current_time();

	bool running = true;
	while (running)
	{
		double now = current_time();
		gd->dt = now - last_time;
		last_time = now;

		update(gd);
		render(&g, gd);

		swap_buffers();
		poll_events();

		running = !get_button(256)->down;
	}
}



static void update(game_data* gd)
{
	game_stack* stack = &gd->stack;
	double dt = gd->dt;
	button* select = get_button(49);
	button* cancel = get_button(50);

	game_state* gs = peek(stack);
	
	if (!select->down && select->transitions > 0)
	{
		push(stack, gs);
		printf("top: %i\n", stack->top);
		printf("bottom: %i\n", stack->bottom);
	}

	if (!cancel->down && cancel->transitions > 0)
	{
		pop(stack);
		printf("top: %i\n", stack->top);
		printf("bottom: %i\n", stack->bottom);
	}

	gs = peek(stack);
}

