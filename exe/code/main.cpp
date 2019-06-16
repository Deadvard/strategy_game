#include <SDL/SDL.h>
#include <glad.c>
#include <stdio.h>

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

	window.gl_context = SDL_GL_CreateContext(window.window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}
	glEnable(GL_DEPTH_TEST);

	return window;
}

void(*tick)(void);

int main(int argc, char** argv)
{
	void* game_code = SDL_LoadObject("dll.dll");

	tick = (void(*)(void))SDL_LoadFunction(game_code, "tick");

	window window = create_window("game", 1280, 720);

	double last_time = (double)SDL_GetTicks() / 1000.0;
	double delta_time = 0.0;

	bool running = true;

	while (running)
	{
		double now = (double)SDL_GetTicks() / 1000.0;
		delta_time = now - last_time;
		last_time = now;

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
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
				break;
			}
			}
		}

		
		if (tick) tick();

		glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window.window);
	}

	SDL_UnloadObject(game_code);
	
	return 0;
}