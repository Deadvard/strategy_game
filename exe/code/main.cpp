#include <SDL/SDL.h>
#include <glad.c>
#include <stdio.h>

#include "renderer.h"
#include "../../dll/code/game.h"

struct window
{
	SDL_Window* window;
	SDL_GLContext gl_context;
};

window create_window(const char* title, int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window window = {};

	window.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	if (!window.window)
	{
		printf("Unable to create window\n");
		printf("SLD Error : %s\n",SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetSwapInterval(1);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	window.gl_context = SDL_GL_CreateContext(window.window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}
	glEnable(GL_DEPTH_TEST);

	return window;
}

typedef void(*tickfunc)(game_memory*, game_input*);

int main(int argc, char** argv)
{
	void* game_code = SDL_LoadObject("dll.dll");

	tickfunc tick = (void(*)(game_memory*, game_input*))SDL_LoadFunction(game_code, "tick");

	window window = create_window("game", 1280, 720);

	renderer rend = {};
	initialize(&rend);

	game_memory memory = {};
	game_input input = {};

	memory.storage = (unsigned char*)malloc(1000);
	memory.storage_size = 1000;
	memset(memory.storage, 0, memory.storage_size);

	double last_time = (double)SDL_GetTicks() / 1000.0;
	double delta_time = 0.0;

	bool running = tick;

	while (running)
	{
		double now = (double)SDL_GetTicks() / 1000.0;
		delta_time = now - last_time;
		last_time = now;

		input.up.transitions = 0;
		input.left.transitions = 0;
		input.down.transitions = 0;
		input.right.transitions = 0;
		input.mouse_x = 0;
		input.mouse_y = 0;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				if (e.key.repeat == 0)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_w:
					{
						input.up.down = !input.up.down;
						++input.up.transitions;
						break;
					}
					case SDLK_a:
					{
						input.left.down = !input.left.down;
						++input.left.transitions;
						break;
					}
					case SDLK_s:
					{
						input.down.down = !input.down.down;
						++input.down.transitions;
						break;
					}
					case SDLK_d:
					{
						input.right.down = !input.right.down;
						++input.right.transitions;
						break;
					}
					case SDLK_ESCAPE:
					{
						running = false;
						break;
					}
					}
				}
				break;
			}
			case SDL_MOUSEMOTION:
			{			
				input.mouse_x = e.motion.xrel;
				input.mouse_y = e.motion.yrel;
				break;
			}
			}
		}

		
		tick(&memory, &input);

		glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render(&memory, &rend);
		SDL_GL_SwapWindow(window.window);
	}

	SDL_UnloadObject(game_code);
	
	return 0;
}