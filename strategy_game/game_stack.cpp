#include "game_stack.h"

void push(game_stack* s, game_state* g)
{
	int top = (s->top + 1);

	if (top >= array_count(s->states))
	{
		top = 0;
	}

	if (top != s->bottom)
	{
		s->top = top;
		s->states[s->top] = *g;
	}
}

void pop(game_stack* s)
{
	int top = (s->top - 1);

	if (top < 0)
	{
		top = array_count(s->states) - 1;
	}

	if (top != s->bottom)
	{
		s->top = top;
	}
}

game_state* peek(game_stack* s)
{
	return s->states + s->top;
}
