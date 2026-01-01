#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "window.h"
#include "event_handler.h"
#include "clock.h"
#include "triangle.h"
#include "loader.h"
#include "object_manager.h"

#define WIDTH 1280
#define HEIGHT 720
#define BG_COLOR 0x00000000

static Camera camera = { { 0, 2, -4 }, 0.0f, 0.0f };

// Update & Render

void Update(float deltaTime)
{
	const uint8_t* keys = SDL_GetKeyboardState(NULL);

	UpdateCamera(deltaTime, &camera, keys, GetMouse().mouseDX, GetMouse().mouseDY);

	// Updating objects
	Mat4* transform = &GetRenderObjectByIndex(1)->transform;
	*transform = Mat4Mul(*transform, Mat4Mul(Mat4RotateX(0.7f * deltaTime), Mat4RotateY(0.7f * deltaTime)));
}

void Render(void)
{
	ClearFrameBuffer(BG_COLOR);
	ClearDepthBuffer();

	ObjectManagerRender();
}

// Main function

int main(int argc, char* args[])
{
	// Initialising
	InitWindow(WIDTH, HEIGHT);
	RendererInit(WIDTH, HEIGHT, &camera);
	ObjectManagerInit();

	WindowCreateCanvas();

	// Loading objects & scene creation	
	AddRenderObject(CreateRenderObjectFromMesh(LoadMeshFromObjFile("res/obj/cube.obj"), Mat4ScaleVec3((Vec3) { 10, 0.2f, 10 }), 0xff00ff00));
	AddRenderObject(CreateRenderObjectFromMesh(LoadMeshFromObjFile("res/obj/cube.obj"), Mat4Translate(1.5f, 2, 2), 0xffff0000));
	AddRenderObject(CreateRenderObjectFromMesh(LoadMeshFromObjFile("res/obj/chair.obj"), Mat4Translate(1, 0.12f, -1), 0xff4444ff));
	AddRenderObject(CreateRenderObjectFromMesh(LoadMeshFromObjFile("res/obj/snowman.obj"), Mat4Translate(-1, 0, 0), 0xffffaaff));
	
	printf("Loaded objects.\n");

	RendererSetLightDir((Vec3) { -0.4, 0.8, -0.5 });

	// Main loop
	
	Clock clock = CreateClock();
	int running = 1;
	while (running)
	{
		if (!EventLoop())
		{
			running = 0;
			break;
		}

		TickClock(&clock);

		Update(clock.deltaTime);
		Render();

		WindowRenderToCanvas(RendererGetFrameBuffer(), RendererGetWidth() * sizeof(uint32_t));
		UpdateWindow();
		
		UpdateEventHandler();
	}

	// Destroying
	ObjectManagerDestroy();
	RendererDestroy();
	WindowDestroy();

	return 0;
}
