#include "window.h"

#include <glad.c>
#include <GLFW/glfw3.h>

static GLFWwindow* window;
static button buttons[512];

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	button b;

	if (action == GLFW_PRESS)
	{
		buttons[key].down = true;
		++buttons[key].transitions;
	}
	else if (action == GLFW_RELEASE)
	{
		buttons[key].down = false;
		++buttons[key].transitions;
	}
}

void initialize_window()
{
	if (glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window =
			glfwCreateWindow(1280, 720, "strategy_game", 0, 0);

		if (window)
		{
			glfwMakeContextCurrent(window);

			glfwSetKeyCallback(window, key_callback);

			if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glEnable(GL_CULL_FACE);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

		}
	}
}

void poll_events()
{
	for (int i = 0; i < 512; ++i)
	{
		buttons[i].transitions = 0;
	}

	glfwPollEvents();
}

void swap_buffers()
{
	glfwSwapBuffers(window);
}

button* get_button(int key)
{
	return &buttons[key];
}

unsigned button_down(int key)
{
	return buttons[key].down;
}

unsigned button_transitions(int key)
{
	return buttons[key].transitions;
}

double current_time()
{
	return glfwGetTime();
}
