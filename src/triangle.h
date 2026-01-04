#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <math.h>

#include "line.h"
#include "math.h"
#include "texture.h"

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void DrawFilledTriangleVertices(
	float x0, float y0, float z0, float u0, float v0, float xn0, float yn0, float zn0,
	float x1, float y1, float z1, float u1, float v1, float xn1, float yn1, float zn1,
	float x2, float y2, float z2, float u2, float v2, float xn2, float yn2, float zn2,
	Texture texture, Vec3 lightDir
);
void DrawFilledTriangle(Triangle t, Texture texture, Vec3 lightDir);
void DrawTriangleWireframe(Triangle t, uint32_t color);

void SetTriangleVertex(Triangle *t, int index, Vertex v);
void SetTriangleVertexPosition(Triangle *t, int index, Vec3 v);
Vertex GetTriangleVertex(Triangle t, int index);

Triangle ProjectTriangle(Triangle t);

int IsTriangleBackFacing(Triangle t);

int ClipTriangleNear(Triangle t, Triangle out[2]);

Vec3 ComputeTriangleNormal(Triangle t);

void PrintTriangle(Triangle t);

#endif
