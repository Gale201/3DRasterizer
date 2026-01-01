#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <SDL2/SDL.h>

#include "event_handler.h"

void InitWindow(int width, int height);
void WindowCreateCanvas(void);
void UpdateWindow(void);
void WindowRenderToCanvas(uint32_t* frameBuffer, size_t size);
void WindowDestroy(void);
SDL_Window* GetWindow(void);

#endif
