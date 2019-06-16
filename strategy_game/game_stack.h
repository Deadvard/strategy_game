#ifndef GAME_STACK_H
#define GAME_STACK_H

#define array_count(array) (sizeof(array) / sizeof((array)[0]))

enum
{
	char_idle,
	char_selected,
	char_moved,
	char_done
};

enum
{
	move_up = 1,
	move_left = 2,
	move_down = 4,
	move_right = 8
};

typedef struct
{
	short type;
	short uses;
} item;

typedef struct
{
	const char* name;
	int x;
	int y;

	item items[5];
	char equipped;

	char health;
	char strength;
	char magic;
	char skill;
	char speed;
	char luck;
	char defence;
	char resistance;
	char movement;

	char path[480];
} character;

typedef struct
{
	character characters[100];
	item inventory[1000];
	character* selected;
} game_state;

typedef struct
{
	game_state states[100];
	int top;
	int bottom;
} game_stack;

typedef struct
{
	double dt;
	short tilemap[1000];
	int x;
	int y;
	game_stack stack;
} game_data;

void push(game_stack* s, game_state* g);
void pop(game_stack* s);
game_state* peek(game_stack* s);

#endif
