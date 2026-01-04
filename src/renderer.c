#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <stdio.h>

#include "renderer.h"

static uint32_t *frameBuffer = NULL;
static int bufferWidth = 0;
static int bufferHeight = 0;

static float *depthBuffer = NULL;

static Camera *camera;

static Vec3 lightDir = { 0, 0, -1 };

/* Low level stuff */

void RendererInit(int width, int height, Camera *cam)
{
	bufferWidth = width;
	bufferHeight = height;
	camera = cam;

	frameBuffer = malloc(sizeof(uint32_t) * bufferWidth * bufferHeight);
	depthBuffer = malloc(sizeof(float) * bufferWidth * bufferHeight);
}

void RendererDestroy(void)
{
	free(frameBuffer);
	frameBuffer = NULL;
	
	free(depthBuffer);
	depthBuffer = NULL;
}

void ClearFrameBuffer(uint32_t color)
{
	for (int i = 0; i < bufferWidth * bufferHeight; i++)
		frameBuffer[i] = color;
}

void ClearDepthBuffer(void)
{
	for (int i = 0; i < bufferWidth * bufferHeight; i++)
		depthBuffer[i] = 0.0f;
}

void DrawPixel(int x, int y, uint32_t color)
{
	if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight) return;

	frameBuffer[y * bufferWidth + x] = color;
}

void DrawDepthPixel(int x, int y, float z, uint32_t color)
{
	if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight) return;
	if (z <= 0) return;

	int index = y * bufferWidth + x;

	if (z > depthBuffer[index] + 1e-6f)
	{
		frameBuffer[index] = color;
		depthBuffer[index] = z;
	}
}

/* Other rendering */

uint32_t ShadeColor(uint32_t color, float intensity)
{
	if (intensity < 0.0f) intensity = 0.0f;
	else if (intensity > 1.0f) intensity = 1.0f;

	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = color & 0xFF;

	r = (uint8_t) (r * intensity);
	g = (uint8_t) (g * intensity);
	b = (uint8_t) (b * intensity);

	return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

uint32_t ComputeShadedColorForTriangle(Triangle t, Vec3 light, uint32_t color)
{
	Vec3 normal = ComputeTriangleNormal(t);

	float intensity = Vec3Dot(normal, light);
	if (intensity < 0.1f) intensity = 0.1f;	

	return ShadeColor(color, intensity);
}

uint32_t ComputeShadedColorForNormal(Vec3 normal, Vec3 light, uint32_t color)
{
	float intensity = Vec3Dot(normal, light);
	//if (intensity < 0.1f) intensity = 0.1f;

	return ShadeColor(color, intensity + 0.15f);	
}

Vec3 TransformDirectionalLight(Vec3 light, Mat4 view)
{
	Vec4 v = { light.x, light.y, light.z, 0 };
	Vec4 r = Mat4MulVec4(view, v);
	return Vec3Normalize((Vec3) { r.x, r.y, r.z });
}

void RenderMesh(const Mesh* mesh, Mat4 transform, Texture texture)
{
	Mat4 view = CameraGetViewMatrix(camera);

	Vec3 transformedLightDir = TransformDirectionalLight(lightDir, view); 	

	for (int i = 0; i < mesh->indexCount; i += 3)
	{
		// Create current triangle
		Triangle t;
		for (int j = 0; j < 3; j++)
		{
			Vertex v = mesh->vertices[mesh->indices[i + j]];

			Vec4 v4 = { v.position.x, v.position.y, v.position.z, 1.0f };
			Vec4 wv4 = Mat4MulVec4(transform, v4);
			Vec4 cv4 = Mat4MulVec4(view, wv4);
			Vec3 cameraVertex = { cv4.x, cv4.y, cv4.z };

			Vec4 n4 = { v.normal.x, v.normal.y, v.normal.z, 0.0f };
			Vec4 wn4 = Mat4MulVec4(transform, n4);
			Vec3 worldNormal = { wn4.x, wn4.y, wn4.z };
			worldNormal = Vec3Normalize(worldNormal);

			Vertex screenVertex = { cameraVertex, worldNormal, v.u, v.v };

			SetTriangleVertex(&t, j, screenVertex);
		}

		// Back-face culling
		if (IsTriangleBackFacing(t))
			continue;
		
		// Clipping
		Triangle clipped[2];
		int n = ClipTriangleNear(t, clipped);
		//uint32_t shadedColor = ComputeShadedColorForTriangle(t, transformedLightDir, color);
		
		// Projecting & Drawing
		for (int j = 0; j < n; j++)
		{
			Triangle projected = ProjectTriangle(clipped[j]);
	
			DrawFilledTriangle(projected, texture, lightDir);
		}
	}
}

void RenderObject(const RObject* obj)
{
	RenderMesh(obj->mesh, obj->transform, obj->texture);
}


/* Getters & Setters */

uint32_t* RendererGetFrameBuffer(void)
{
	return frameBuffer;
}

float* RendererGetDepthBuffer(void)
{
	return depthBuffer;
}

int RendererGetWidth(void)
{
	return bufferWidth;
}

int RendererGetHeight(void)
{
	return bufferHeight;
}

Camera RendererGetCamera(void)
{
	return *camera;
}

Vec3 RendererGetLightDir(void)
{
	return lightDir;
}

void RendererSetCamera(Camera *cam)
{
	camera = cam;
}

void RendererSetLightDir(Vec3 ld)
{
	lightDir = ld;
}
