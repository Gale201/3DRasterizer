#include "event_handler.h"

static Mouse mouse;

static uint8_t lastKeyboardState[SDL_NUM_SCANCODES];

int EventLoop(void)
{
	SDL_Event event;
	int mouseDX = 0, mouseDY = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return 0;
			case SDL_MOUSEMOTION:
				mouseDX += event.motion.xrel;
				mouseDY += event.motion.yrel;
			default:
				break;
		}
	}
	
	mouse.mouseDX = mouseDX;
	mouse.mouseDY = mouseDY;

	return 1;
}

void UpdateEventHandler(void)
{
	const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		lastKeyboardState[i] = keyboardState[i];
	}
}

Mouse GetMouse(void)
{
	return mouse;
}

int IsKeyJustPressed(SDL_KeyCode key)
{
	return !lastKeyboardState[key] && SDL_GetKeyboardState(NULL)[key];
}
