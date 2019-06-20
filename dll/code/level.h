#ifndef LEVEL_H
#define LEVEL_H

struct Level
{
	int* levelData;
};

void loadLevelData(Level* level, const char* path);

#endif