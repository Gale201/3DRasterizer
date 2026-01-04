#ifndef LINE_H
#define LINE_H

#include "renderer.h"
#include "texture.h"

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);
void DrawDepthLine(int x0, int y0, float z0, int x1, int y1, float z1, uint32_t color);
void DrawHorizontalDepthLine(float x0, float x1, int y, float inv_z0, float inv_z1, uint32_t color);
void DrawHorizontalDepthTextureLine(
	float x0, float x1, int y, float inv_z0, float inv_z1, 
	float u0_over_z, float u1_over_z, 
	float v0_over_z, float v1_over_z,
	float xn0_over_z, float xn1_over_z,
	float yn0_over_z, float yn1_over_z,
	float zn0_over_z, float zn1_over_z,
   	Texture texture, Vec3 lightDir);

#endif
