#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <SDL2/SDL.h>

#include "math.h"

typedef struct {
	Vec3 position;
	float yaw;
	float pitch;
} Camera;

#define NEAR_PLANE 0.1f

Vec3 CameraForward(Camera* cam);
Vec3 CameraRight(Camera* cam);
Vec3 CameraUp(Camera* cam);
void UpdateCamera(float deltaTime, Camera *cam, const uint8_t *keys, int mouseDX, int mouseDY);

Vec3 WorldToCameraView(Vec3 v, Camera cam);
Mat4 CameraGetViewMatrix(const Camera* cam);

#endif
