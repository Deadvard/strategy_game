#ifndef WINDOW_H
#define WINDOW_H

typedef struct
{
	unsigned down;
	unsigned transitions;
} button;

void initialize_window();
void poll_events();
void swap_buffers();

button* get_button(int key);

unsigned button_down(int key);
unsigned button_transitions(int key);

double current_time();

void temp();

#endif

