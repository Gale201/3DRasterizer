#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL.h>

typedef struct {
	int mouseX, mouseY, mouseDX, mouseDY;
} Mouse;

int EventLoop(void);
void UpdateEventHandler(void);

Mouse GetMouse(void);
int IsKeyJustPressed(SDL_KeyCode key);

#endif
