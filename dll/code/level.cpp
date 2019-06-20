#include "level.h"
#include <stdio.h>
#include <memory>

void loadLevelData(Level* level, const char * path)
{
	level->levelData = new int[256 * 256];
	memset(level->levelData, 0, sizeof(int) * 256 * 256);
	FILE* f;
	fopen_s(&f, path, "rb");

	int number = 0;
	int index = 0;

	while (fscanf_s(f, "%d", &number) != EOF)
	{
		level->levelData[index++] = number;
	}

	fclose(f);

	for (int i = 0; i < 256 * 256; ++i)
	{
		if(level->levelData[i] != 0) printf("%d", level->levelData[i]);
	}
}
