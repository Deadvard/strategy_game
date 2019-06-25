#include "level.h"
#include <stdio.h>
#include <memory>

typedef struct image
{
	int width;
	int height;
	int components;
	unsigned char* data;
};

void load_textures(const char* path, image** images, int* count)
{
	FILE* f;
	fopen_s(&f, path, "rb");
	fread(count, sizeof(int), 1, f);

	*images = (image*)malloc(sizeof(image) * (*count));

	for (int i = 0; i < *count; ++i)
	{
		image img = {};
		fread(&img.width, sizeof(int), 1, f);
		fread(&img.height, sizeof(int), 1, f);
		fread(&img.components, sizeof(int), 1, f);
		unsigned int size = img.width * img.height * img.components;
		img.data = (unsigned char*)malloc(sizeof(unsigned char) * size);
		fread(img.data, sizeof(unsigned char), size, f);

		*images[i] = img;
	}

	fclose(f);
}