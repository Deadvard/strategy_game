#include "memory.h"

byte* push(memory_stack* m, unsigned int size)
{
	unsigned char* result = 0;
	
	if (m->top + size < m->bottom + m->size)
	{
		result = m->top;
		m->top += size;
	}
	
	return result;
}

unsigned int pop(memory_stack* m, byte* data)
{
	unsigned int result = 0;
	
	if (data >= m->bottom && data < m->top)
	{
		m->top = data;
	}

	return 0;
}