#include <stdio.h> 

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_truetype.h>

typedef struct character
{
	int advance;
	int width;
	int height;
	int xoffset;
	int yoffset;
	unsigned char bitmap[32 * 32];
} character;

character characters[128];

unsigned char ttf_buffer[1 << 25];

int main(int argc, char** argv)
{	
	stbtt_fontinfo font;

	FILE* ff;
	fopen_s(&ff, "font.ttf", "rb");

	fread(ttf_buffer, 1, 1 << 25, ff);
	stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

	FILE* binfont;
	fopen_s(&binfont, "font.bin", "wb");
	for (int c = 0; c < 128; ++c)
	{
		character ch = {};
		unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0,
			stbtt_ScaleForPixelHeight(&font, 32.0f), c, &ch.width, &ch.height, &ch.xoffset, &ch.yoffset);

		memcpy(ch.bitmap, bitmap, ch.width * ch.height);

		stbtt_GetCodepointHMetrics(&font, c, &ch.advance, 0);

		fwrite(&ch, sizeof(character), 1, binfont);

		stbtt_FreeBitmap(bitmap, 0);
	}

	fclose(ff);
	fclose(binfont);
	
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