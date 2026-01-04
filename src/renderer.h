#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "math.h"
#include "mesh.h"
#include "triangle.h"
#include "camera.h"
#include "texture.h"

typedef struct {
	Mesh *mesh;
	Mat4 transform;
	Texture texture;
} RObject;

void RendererInit(int width, int height, Camera* cam);
void RendererDestroy(void);

void ClearFrameBuffer(uint32_t color);
void ClearDepthBuffer(void);

void DrawPixel(int x, int y, uint32_t color);
void DrawDepthPixel(int x, int y, float z, uint32_t color);

uint32_t ShadeColor(uint32_t color, float intensity);
uint32_t ComputeShadedColorForTriangle(Triangle t, Vec3 light, uint32_t color);
uint32_t ComputeShadedColorForNormal(Vec3 normal, Vec3 light, uint32_t color);

void RenderMesh(const Mesh* mesh, Mat4 model, Texture texture);
void RenderObject(const RObject* obj);

uint32_t* RendererGetFrameBuffer(void);
float* RendererGetDepthBuffer(void);
int RendererGetWidth(void);
int RendererGetHeight(void);
Camera RendererGetCamera(void);
Vec3 RendererGetLightDir(void);
void RendererSetCamera(Camera *cam);
void RendererSetLightDir(Vec3 ld);

#endif
