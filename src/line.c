#include <stdio.h>
#include <math.h>

#include "line.h"

static void L_Clip(int p, int q, float *t0, float *t1)
{
	if (p == 0.0f) return;

	float r = q / p;

	if (p < 0.0f)
	{
		if (r > *t1) return;
		if (r > *t1) *t0 = r;
	}
	else
	{
		if (r < *t0) return;
		if (r < *t1) *t1 = r;
	}
}

static void L_ClipLine(int *x0, int *y0, int *x1, int *y1)
{
	float t0 = 0.0f, t1 = 1.0f;
	float dx = *x1 - *x0;
	float dy = *y1 - *y0;

	L_Clip(-dx, *x0, &t0, &t1);
	L_Clip(dx, RendererGetWidth() - 1 - *x0, &t0, &t1);
	L_Clip(-dy, *y0, &t0, &t1);
	L_Clip(dy, RendererGetHeight() - 1 - *y0, &t0, &t1);

	if (t1 < t0) return;

	*x1 = *x0 + t1 * dx;
	*y1 = *y0 + t1 * dy;
	*x0 = *x0 + t0 * dx;
	*y0 = *y0 + t0 * dy;
}

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
	if (x0 > RendererGetWidth() && x1 > RendererGetWidth() || x0 < 0 && x1 < 0) return;
	if (y0 > RendererGetHeight() && y1 > RendererGetHeight() || y0 < 0 && y1 < 0) return;

//	printf("%d %d %d %d\n", x0, y0, x1, y1);
//	L_ClipLine(&x0, &y0, &x1, &y1);
//	printf("%d %d %d %d\n\n", x0, y0, x1, y1);
	
	int dx = x1 - x0;
	int dy = y1 - y0;

	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	float xStep = dx / (float) steps;
	float yStep = dy / (float) steps;

	float x = x0, y = y0;

	for (int i = 0; i <= steps; i++)
	{
		DrawPixel((int) roundf(x), (int) roundf(y), color);
		x += xStep;
		y += yStep;
	}
}

void DrawDepthLine(int x0, int y0, float z0, int x1, int y1, float z1, uint32_t color)
{
	float dx = (float) (x1 - x0) / (float) (y1 - y0);
	float dz = (z1 - z0) / (float) (y1 - y0);
	
	float x = x0;
	float z = z0;

	for (int y = y0; y < y1; y++)
	{
		float x2 = x + dx;
		float z2 = z + dz;
	
		DrawHorizontalDepthLine(x, x2, y, z, z2, color);

		x = x2;
		z = z2;
	}
}

void DrawHorizontalDepthLine(float x0, float x1, int y, float inv_z0, float inv_z1, uint32_t color)
{
	if (y < 0 || y >= RendererGetHeight()) return;
	if (inv_z0 < 0.0f || inv_z1 < 0.0f) return;

	if (x0 > x1)
	{
		int tmpx = x0;
		x0 = x1;
		x1 = tmpx;
		float tmpz = inv_z0;
		inv_z0 = inv_z1;
		inv_z1 = tmpz;
	}

	float xl = x0;
	float xr = x1;
	float inv_zl = inv_z0;
	float inv_zr = inv_z1;
	
	if (xl < -0.5f)
	{
		float t = (-0.5f - xl) / (float) (xr - xl);
		inv_zl += t * (inv_zr - inv_zl);
		xl = -0.5f;
	}
	else if (xr >= RendererGetWidth() - 0.5f)
	{
		float t = (RendererGetWidth() - 0.5f - xl) / (float) (xr - xl);
		inv_zr = inv_zl + t * (inv_zr - inv_zl);
		xr = RendererGetWidth() - 0.5f;
	}

	int x_start = (int) ceilf(xl - 0.5f);
	int x_end = (int) ceilf(xr - 0.5f);

	int dx = xr - xl;
	if (dx <= 1e-3f)
	{
		int x = (int) floorf((xl + xr) * 0.5f);
		DrawDepthPixel(x, y, (inv_zl + inv_zr) / 2, color);
		return;
	}

	float dinv_z = (inv_zr - inv_zl) / dx;
	float inv_z = inv_zl + (x_start + 0.5f - xl) * dinv_z;

	for (int x = x_start; x <= x_end; x++)
	{
		DrawDepthPixel(x, y, inv_z, color);
		inv_z += dinv_z;
	}	
}
