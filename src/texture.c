#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture LoadTexture(const char* filename)
{
	Texture tex = { };

	int w, h, channels;
	unsigned char* data = stbi_load(filename, &w, &h, &channels, 4);

	if (!data)
	{
		printf("Failed to load texture: %s\n", filename);
		return tex;
	}

	tex.width = w;
	tex.height = h;
	tex.texture = (uint32_t*) malloc(w * h * sizeof(uint32_t));

	for (int i = 0; i < w * h; i++)
	{
		unsigned char r = data[i * 4];
		unsigned char g = data[i * 4 + 1];
		unsigned char b = data[i * 4 + 2];
		unsigned char a = data[i * 4 + 3];

		tex.texture[i] = (a << 24) | (r << 16) | (g << 8) | b;
	}

	stbi_image_free(data);
	return tex;
}

uint32_t GetTextureAtUV(Texture texture, float u, float v)
{
	int tx = (int) (u * texture.width);
	int ty = (int) (v * texture.height);
	if (tx < 0) tx = 0;
	else if (tx > texture.width - 1) tx = texture.width - 1;
	if (ty < 0) ty = 0;
	else if (ty > texture.height - 1) ty = texture.height - 1;

	uint8_t r = (uint8_t) (255.0f * u);
	uint8_t b = (uint8_t) (255.0f * v);
	//return 0xff << 24 | (r << 16) | b;

	return texture.texture[ty * texture.width + tx];
}

void DestroyTexture(Texture *texture)
{
	free(texture->texture);
	texture->texture = NULL;
}

