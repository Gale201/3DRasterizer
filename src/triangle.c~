#include <stdio.h>

#include "triangle.h"

#define EPSILON 1e-6f

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	DrawLine(x0, y0, x1, y1, color);
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x0, y0, color);
}

void DrawDepthTriangle(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, uint32_t color)
{
	DrawDepthLine(x0, y0, z0, x1, y1, z1, color);
	DrawDepthLine(x1, y1, z1, x2, y2, z2, color);
	DrawDepthLine(x2, y2, z2, x0, y0, z0, color);
}

static void swap_int(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static void swap_float(float *a, float *b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}

static void T_SortVerticesByY(int *x0, int *y0, float *z0, int *x1, int *y1, float *z1, int *x2, int *y2, float *z2)
{
	if (*y0 > *y1)
	{
		swap_int(y0, y1);
		swap_int(x0, x1);
		swap_float(z0, z1);
	}
	if (*y0 > *y2)
	{
		swap_int(y0, y2);
		swap_int(x0, x2);
		swap_float(z0, z2);
	}
	if (*y1 > *y2)
	{
		swap_int(y1, y2);
		swap_int(x1, x2);
		swap_float(z1, z2);
	}
}

static void T_FillFlatBottomTriangle(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, uint32_t color)
{
	float slope_x1 = (float) (x1 - x0) / (y1 - y0);
	float slope_x2 = (float) (x2 - x0) / (y2 - y0);

	float slope_z1 = (z1 - z0) / (y1 - y0);
	float slope_z2 = (z2 - z0) / (y2 - y0);

	for (int y = y0; y < y1; y++)
	{
		float py = y + 0.5f;
		float dy = py - y0;

		float xl = x0 + dy * slope_x1;
		float xr = x0 + dy * slope_x2;
		float zl = z0 + dy * slope_z1;
		float zr = z0 + dy * slope_z2;

		if (xl > xr)
		{
			swap_float(&xl, &xr);
			swap_float(&zl, &zr);
		}
		
		DrawHorizontalDepthLine(xl, xr, y, zl, zr, color);
	}
}

static void T_FillFlatTopTriangle(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, uint32_t color)
{
	float slope_x1 = (float) (x2 - x0) / (y2 - y0);
	float slope_x2 = (float) (x2 - x1) / (y2 - y1);

	float slope_z1 = (z2 - z0) / (y2 - y0);
	float slope_z2 = (z2 - z1) / (y2 - y1);

	for (int y = y2 - 1; y >= y0; y--)
	{	
		float py = y + 0.5f;
		float dy0 = py - y0;
		float dy1 = py - y1;

		float xl = x0 + dy0 * slope_x1;
		float xr = x1 + dy1 * slope_x2;
		float zl = z0 + dy0 * slope_z1;
		float zr = z1 + dy1 * slope_z2;
	
		if (xl > xr)
		{
			swap_float(&xl, &xr);
			swap_float(&zl, &zr);
		}	

		DrawHorizontalDepthLine(xl, xr, y, zl, zr, color);
	}
}

void DrawFilledTriangleVertices(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, uint32_t color)
{
	T_SortVerticesByY(&x0, &y0, &z0, &x1, &y1, &z1, &x2, &y2, &z2);

	if (y1 == y2) T_FillFlatBottomTriangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, color);
	else if (y0 == y1) T_FillFlatTopTriangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, color);
	else
	{
		float alpha = (float) (y1 - y0) / (y2 - y0);
		int x3 = (int) (x0 + alpha * (x2 - x0));
		float z3 = z0 + alpha * (z2 - z0);
		int y3 = y1;

		T_FillFlatBottomTriangle(x0, y0, z0, x1, y1, z1, x3, y3, z3, color);
		T_FillFlatTopTriangle(x1, y1, z1, x3, y3, z3, x2, y2, z2, color);
	}
}

void DrawFilledTriangle(Triangle t, uint32_t color)
{
	DrawFilledTriangleVertices((int) t.v0.x, (int) t.v0.y, 1.0f / t.v0.z, (int) t.v1.x, (int) t.v1.y, 1.0f / t.v1.z, (int) t.v2.x, (int) t.v2.y, 1.0f / t.v2.z, color);
	//DrawTriangle((int) t.v0.x, (int) t.v0.y, (int) t.v1.x, (int) t.v1.y, (int) t.v2.x, (int) t.v2.y, 0xff0000);
    //DrawDepthTriangle((int) t.v0.x, (int) t.v0.y, 1.0f / t.v0.z, (int) t.v1.x, (int) t.v1.y, 1.0f / t.v1.z, (int) t.v2.x, (int) t.v2.y, 1.0f / t.v2.z, 0xffff0000);
}

Vec3 T_IntersectNear(Vec3 a, Vec3 b)
{
	if (fabsf(b.z - a.z) < EPSILON) return (Vec3) { a.x, a.y, NEAR_PLANE };

	float t = (NEAR_PLANE - a.z) / (b.z - a.z);

	Vec3 r;
	r.x = a.x + t * (b.x - a.x);
	r.y = a.y + t * (b.y - a.y);
	r.z = NEAR_PLANE;

	return r;
}

void SetTriangleVertex(Triangle *t, int index, Vec3 v)
{
	if (index == 0) t->v0 = v;
	else if (index == 1) t->v1 = v;
	else if (index == 2) t->v2 = v;
}

Vec3 GetTriangleVertex(Triangle t, int index)
{
	if (index == 0) return t.v0;
	else if (index == 1) return t.v1;
	return t.v2;
}

Triangle ProjectTriangle(Triangle t)
{
	int x, y;
	float z;

	Triangle projected;

	for (int i = 0; i < 3; i++)
	{
		ProjectVertex(GetTriangleVertex(t, i), &x, &y, &z);
		Vec3 v = { x, y, z };
		SetTriangleVertex(&projected, i, v);
	}

	return projected;
}

int IsTriangleBackFacing(Triangle t)
{
	Vec3 e1 = Vec3Sub(t.v1, t.v0);
	Vec3 e2 = Vec3Sub(t.v2, t.v0);

	Vec3 normal = Vec3Cross(e1, e2);

	Vec3 view = { -t.v0.x, -t.v0.y, -t.v0.z };

	return Vec3Dot(normal, view) <= 0.0f;
}

int ClipTriangleNear(Triangle t, Triangle out[2])
{
	Vec3 v[3] = { t.v0, t.v1, t.v2 };
	Vec3 inside[3];
	Vec3 outside[3];
	int inCount = 0, outCount = 0;

	for (int i = 0; i < 3; i++)
	{
		if (v[i].z >= NEAR_PLANE)
			inside[inCount++] = v[i];
		else
			outside[outCount++] = v[i];
	}

	if (inCount == 0) return 0;
	if (inCount == 3)
	{
		out[0] = t;
		return 1;
	}

	if (inCount == 1)
	{
		Vec3 A = inside[0], B = outside[0], C = outside[1];	

		Vec3 i1 = T_IntersectNear(A, B);
		Vec3 i2 = T_IntersectNear(A, C);

		out[0] = (Triangle) { A, i1, i2 };

		return 1;
	}

	if (inCount == 2)
	{
		Vec3 A = inside[0];
		Vec3 B = inside[1];
		Vec3 C = outside[0];
	
		Vec3 i1 = T_IntersectNear(A, C);
		Vec3 i2 = T_IntersectNear(B, C);

		out[0] = (Triangle) { A, B, i1 };
		out[1] = (Triangle) { B, i2, i1 };

		return 2;
	}

	return 0;
}

Vec3 ComputeTriangleNormal(Triangle t)
{
	Vec3 e1 = Vec3Sub(t.v1, t.v0);
	Vec3 e2 = Vec3Sub(t.v2, t.v0);
	return Vec3Normalize(Vec3Cross(e1, e2));
}

void PrintTriangle(Triangle t)
{
	printf("Triangle(v0={ %f, %f, %f }, v1={ %f, %f, %f }, v2={ %f, %f, %f })", t.v0.x, t.v0.y, t.v0.z, t.v1.x, t.v1.y, t.v1.z, t.v2.x, t.v2.y, t.v2.z);
}
