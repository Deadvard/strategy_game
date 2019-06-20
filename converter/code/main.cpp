#include <stdio.h> 

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_truetype.h>

int main(int argc, char** argv)
{
	FILE* f;
	fopen_s(&f, "textures.bin", "wb");
	int textures = argc - 1;
	fwrite(&textures, sizeof(int), 1, f);
	
	for (int i = 1; i < argc; ++i)
	{
		int width, height, components;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(argv[i], &width, &height, &components, 0);
		
		fwrite(&width, sizeof(int), 1, f);
		fwrite(&height, sizeof(int), 1, f);
		fwrite(&components, sizeof(int), 1, f);
		fwrite(data, sizeof(unsigned char), width * height * components, f);
		stbi_image_free(data);
	}

	fclose(f);
	
	return 0;
}