#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned char byte;

struct memory_stack
{
	byte* bottom;
	byte* top;
	unsigned int size;	
};

byte* push(memory_stack* m, unsigned int size);
unsigned int pop(memory_stack* m, byte* data);


#endif
