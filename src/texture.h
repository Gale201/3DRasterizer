#ifndef TEXTURE_H
#define TEXTURE_H

#include "stdint.h"

typedef struct {
	int width, height;
	uint32_t* texture;
} Texture;

Texture LoadTexture(const char* filename);
uint32_t GetTextureAtUV(Texture texture, float u, float v);
void DestroyTexture(Texture *texture);

#endif
