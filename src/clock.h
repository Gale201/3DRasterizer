#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL.h>

typedef struct {
	float lastTicks;
	float currentTicks;
	float deltaTime;	
} Clock;

Clock CreateClock(void);
void TickClock(Clock* clock);

#endif
