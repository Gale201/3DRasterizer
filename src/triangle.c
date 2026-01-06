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

static void T_SortVerticesByY(
	float *x0, float *y0, float *z0, float *u0, float *v0, float *xn0, float *yn0, float *zn0,
   	float *x1, float *y1, float *z1, float *u1, float *v1, float *xn1, float *yn1, float *zn1,
	float *x2, float *y2, float *z2, float *u2, float *v2, float *xn2, float *yn2, float *zn2
)
{
	if (*y0 > *y1)
	{
		swap_float(y0, y1);
		swap_float(x0, x1);
		swap_float(z0, z1);
		swap_float(u0, u1);
		swap_float(v0, v1);
		swap_float(xn0, xn1);
		swap_float(yn0, yn1);
		swap_float(zn0, zn1);
	}
	if (*y0 > *y2)
	{
		swap_float(y0, y2);
		swap_float(x0, x2);
		swap_float(z0, z2);
		swap_float(u0, u2);
		swap_float(v0, v2);
		swap_float(xn0, xn2);
		swap_float(yn0, yn2);
		swap_float(zn0, zn2);
	}
	if (*y1 > *y2)
	{
		swap_float(y1, y2);
		swap_float(x1, x2);
		swap_float(z1, z2);
		swap_float(u1, u2);
		swap_float(v1, v2);
		swap_float(xn1, xn2);
		swap_float(yn1, yn2);
		swap_float(zn1, zn2);
	}
}

static void T_FillFlatBottomTriangle(
	float x0, float y0, float z0, float u0, float v0, float xn0, float yn0, float zn0,
	float x1, float y1, float z1, float u1, float v1, float xn1, float yn1, float zn1,
	float x2, float y2, float z2, float u2, float v2, float xn2, float yn2, float zn2,
	Texture texture, Vec3 lightDir
)
{
	float slope_x1 = (x1 - x0) / (y1 - y0);
	float slope_x2 = (x2 - x0) / (y2 - y0);

	float slope_z1 = (z1 - z0) / (y1 - y0);
	float slope_z2 = (z2 - z0) / (y2 - y0);

	float slope_u1 = (u1 - u0) / (y1 - y0);
	float slope_u2 = (u2 - u0) / (y2 - y0);

	float slope_v1 = (v1 - v0) / (y1 - y0);
	float slope_v2 = (v2 - v0) / (y2 - y0);

	float slope_xn1 = (xn1 - xn0) / (y1 - y0);
	float slope_xn2 = (xn2 - xn0) / (y2 - y0);

	float slope_yn1 = (yn1 - yn0) / (y1 - y0);
	float slope_yn2 = (yn2 - yn0) / (y2 - y0);

	float slope_zn1 = (zn1 - zn0) / (y1 - y0);
	float slope_zn2 = (zn2 - zn0) / (y2 - y0);
	
	for (int y = y0; y < y1; y++)
	{
		if (y < 0 || y >= RendererGetHeight()) continue;

		float py = y + 0.5f;
		float dy = py - y0;

		float xl = x0 + dy * slope_x1;
		float xr = x0 + dy * slope_x2;
		float zl = z0 + dy * slope_z1;
		float zr = z0 + dy * slope_z2;
		
		float ul = u0 + dy * slope_u1;
		float ur = u0 + dy * slope_u2;
		float vl = v0 + dy * slope_v1;
		float vr = v0 + dy * slope_v2;

		float xnl = xn0 + dy * slope_xn1;
		float xnr = xn0 + dy * slope_xn2;
		float ynl = yn0 + dy * slope_yn1;
		float ynr = yn0 + dy * slope_yn2;
		float znl = zn0 + dy * slope_zn1;
		float znr = zn0 + dy * slope_zn2;

		if (xl > xr)
		{
			swap_float(&xl, &xr);
			swap_float(&zl, &zr);
			swap_float(&ul, &ur);
			swap_float(&vl, &vr);
			swap_float(&xnl, &xnr);
			swap_float(&ynl, &ynr);
			swap_float(&znl, &znr);
		}
		
		DrawHorizontalDepthTextureLine(xl, xr, y, zl, zr, ul, ur, vl, vr, xnl, xnr, ynl, ynr, znl, znr, texture, lightDir);
	}
}

static void T_FillFlatTopTriangle(
	float x0, float y0, float z0, float u0, float v0, float xn0, float yn0, float zn0,
	float x1, float y1, float z1, float u1, float v1, float xn1, float yn1, float zn1,
	float x2, float y2, float z2, float u2, float v2, float xn2, float yn2, float zn2,
	Texture texture, Vec3 lightDir
)
{
	float slope_x1 = (x2 - x0) / (y2 - y0);
	float slope_x2 = (x2 - x1) / (y2 - y1);

	float slope_z1 = (z2 - z0) / (y2 - y0);
	float slope_z2 = (z2 - z1) / (y2 - y1);

	float slope_u1 = (u2 - u0) / (y2 - y0);
	float slope_u2 = (u2 - u1) / (y2 - y1);

	float slope_v1 = (v2 - v0) / (y2 - y0);
	float slope_v2 = (v2 - v1) / (y2 - y1);

	float slope_xn1 = (xn2 - xn0) / (y2 - y0);
	float slope_xn2 = (xn2 - xn1) / (y2 - y1);

	float slope_yn1 = (yn2 - yn0) / (y2 - y0);
	float slope_yn2 = (yn2 - yn1) / (y2 - y1);

	float slope_zn1 = (zn2 - zn0) / (y2 - y0);
	float slope_zn2 = (zn2 - zn1) / (y2 - y1);
	
	for (int y = y2 - 1; y >= y0; y--)
	{
		if (y < 0 || y >= RendererGetHeight()) continue;

		float py = y + 0.5f;
		float dy0 = py - y0;
		float dy1 = py - y1;

		float xl = x0 + dy0 * slope_x1;
		float xr = x1 + dy1 * slope_x2;
		float zl = z0 + dy0 * slope_z1;
		float zr = z1 + dy1 * slope_z2;

		float ul = u0 + dy0 * slope_u1;
		float ur = u1 + dy1 * slope_u2;
		float vl = v0 + dy0 * slope_v1;
		float vr = v1 + dy1 * slope_v2;

		float xnl = xn0 + dy0 * slope_xn1;
		float xnr = xn1 + dy1 * slope_xn2;
		float ynl = yn0 + dy0 * slope_yn1;
		float ynr = yn1 + dy1 * slope_yn2;
		float znl = zn0 + dy0 * slope_zn1;
		float znr = zn1 + dy1 * slope_zn2;

		if (xl > xr)
		{
			swap_float(&xl, &xr);
			swap_float(&zl, &zr);
			swap_float(&ul, &ur);
			swap_float(&vl, &vr);
			swap_float(&xnl, &xnr);
			swap_float(&ynl, &ynr);
			swap_float(&znl, &znr);
		}	

		DrawHorizontalDepthTextureLine(xl, xr, y, zl, zr, ul, ur, vl, vr, xnl, xnr, ynl, ynr, znl, znr, texture, lightDir);
	}
}

void DrawFilledTriangleVertices(
	float x0, float y0, float z0, float u0, float v0, float xn0, float yn0, float zn0,
	float x1, float y1, float z1, float u1, float v1, float xn1, float yn1, float zn1,
	float x2, float y2, float z2, float u2, float v2, float xn2, float yn2, float zn2,
	Texture texture, Vec3 lightDir
)
{
	T_SortVerticesByY(
		&x0, &y0, &z0, &u0, &v0, &xn0, &yn0, &zn0, 
		&x1, &y1, &z1, &u1, &v1, &xn1, &yn1, &zn1,
		&x2, &y2, &z2, &u2, &v2, &xn2, &yn2, &zn2
	);

	if ((int) y1 == (int) y2) 
	{
		T_FillFlatBottomTriangle(
			x0, y0, z0, u0, v0, xn0, yn0, zn0, 
			x1, y1, z1, u1, v1, xn1, yn1, zn1,
			x2, y2, z2, u2, v2, xn2, yn2, zn2,
			texture, lightDir
		);
	}
	else if ((int) y0 == (int) y1) 
	{
		T_FillFlatTopTriangle(
			x0, y0, z0, u0, v0, xn0, yn0, zn0,
			x1, y1, z1, u1, v1, xn1, yn1, zn1,
			x2, y2, z2, u2, v2, xn2, yn2, zn2,
			texture, lightDir
		);
	}
	else
	{
		float alpha = (y1 - y0) / (y2 - y0);

		float x3 = x0 + alpha * (x2 - x0);
		float z3 = z0 + alpha * (z2 - z0);
		float y3 = y1;

		float u3 = u0 + alpha * (u2 - u0);
		float v3 = v0 + alpha * (v2 - v0);
		
		float xn3 = xn0 + alpha * (xn2 - xn0);
		float yn3 = yn0 + alpha * (yn2 - yn0);
		float zn3 = zn0 + alpha * (zn2 - zn0);

		T_FillFlatBottomTriangle(
			x0, y0, z0, u0, v0, xn0, yn0, zn0,
			x1, y1, z1, u1, v1, xn1, yn1, zn1,
			x3, y3, z3, u3, v3, xn3, yn3, zn3,
			texture, lightDir
		);
		T_FillFlatTopTriangle(
			x1, y1, z1, u1, v1, xn1, yn1, zn1,
			x3, y3, z3, u3, v3, xn3, yn3, zn3,
			x2, y2, z2, u2, v2, xn2, yn2, zn2,
			texture, lightDir
		);
	}
}

void DrawFilledTriangle(Triangle t, Texture texture, Vec3 lightDir)
{
	Vec3 v0 = t.v0.position;
	Vec3 v1 = t.v1.position;
	Vec3 v2 = t.v2.position;
	
	Vec3 vn0 = Vec3Scale(t.v0.normal, 1.0f / v0.z);
	Vec3 vn1 = Vec3Scale(t.v1.normal, 1.0f / v1.z);
	Vec3 vn2 = Vec3Scale(t.v2.normal, 1.0f / v2.z);
	
	DrawFilledTriangleVertices(
		(int) v0.x, (int) v0.y, 1.0f / v0.z, t.v0.u, t.v0.v, vn0.x, vn0.y, vn0.z,
		(int) v1.x, (int) v1.y, 1.0f / v1.z, t.v1.u, t.v1.v, vn1.x, vn1.y, vn1.z,
		(int) v2.x, (int) v2.y, 1.0f / v2.z, t.v2.u, t.v2.v, vn2.x, vn2.y, vn2.z,
		texture, lightDir
	);
    //DrawDepthTriangle((int) t.v0.x, (int) t.v0.y, 1.0f / t.v0.z, (int) t.v1.x, (int) t.v1.y, 1.0f / t.v1.z, (int) t.v2.x, (int) t.v2.y, 1.0f / t.v2.z, 0xffff0000);
}

void DrawTriangleWireframe(Triangle t, uint32_t color)
{
	Vec3 v0 = t.v0.position;
	Vec3 v1 = t.v1.position;
	Vec3 v2 = t.v2.position;
	DrawTriangle((int) v0.x, (int) v0.y, (int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
}

void SetTriangleVertex(Triangle *t, int index, Vertex v)
{
	if (index == 0) t->v0 = v;
	else if (index == 1) t->v1 = v;
	else if (index == 2) t->v2 = v;
}

void SetTriangleVertexPosition(Triangle *t, int index, Vec3 v)
{
	if (index == 0) t->v0.position = v;
	else if (index == 1) t->v1.position = v;
	else if (index == 2) t->v2.position = v;
}

Vertex GetTriangleVertex(Triangle t, int index)
{
	if (index == 0) return t.v0;
	else if (index == 1) return t.v1;
	return t.v2;
}

Triangle ProjectTriangle(Triangle t)
{
	Triangle projected;

	for (int i = 0; i < 3; i++)
	{
		Vertex v = GetTriangleVertex(t, i);
		v = ProjectVertex(&v);
		SetTriangleVertex(&projected, i, v);
	}

	return projected;
}

int IsTriangleBackFacing(Triangle t)
{
	Vec3 e1 = Vec3Sub(t.v1.position, t.v0.position);
	Vec3 e2 = Vec3Sub(t.v2.position, t.v0.position);

	Vec3 normal = Vec3Cross(e1, e2);

	Vec3 view = { -t.v0.position.x, -t.v0.position.y, -t.v0.position.z };

	return Vec3Dot(normal, view) <= 0.0f;
}

Vertex T_IntersectNear(Vertex a, Vertex b)
{
	if (fabsf(b.position.z - a.position.z) < EPSILON) return (Vertex) { a.position.x, a.position.y, NEAR_PLANE, a.u / NEAR_PLANE, a.v / NEAR_PLANE };

	float t = (NEAR_PLANE - a.position.z) / (b.position.z - a.position.z);

	float u = a.u + t * (b.u - a.u);
	float v = a.v + t * (b.v - a.v);

	Vertex r;
	r.position.x = a.position.x + t * (b.position.x - a.position.x);
	r.position.y = a.position.y + t * (b.position.y - a.position.y);
	r.position.z = NEAR_PLANE;
	r.u = u / NEAR_PLANE;
	r.v = v / NEAR_PLANE;
	r.normal = Vec3Add(a.normal, Vec3Scale(Vec3Sub(b.normal, a.normal), t));

	return r;
}

int ClipTriangleNear(Triangle t, Triangle out[2])
{
	Vertex v[3] = { t.v0, t.v1, t.v2 };
	Vertex inside[3];
	Vertex outside[3];
	int inCount = 0, outCount = 0;

	for (int i = 0; i < 3; i++)
	{
		if (v[i].position.z >= NEAR_PLANE)
			inside[inCount++] = v[i];
		else
			outside[outCount++] = v[i];
	}

	if (inCount == 0) return 0;
	if (inCount == 3)
	{
		out[0] = (Triangle) {
		   	{ { t.v0.position.x, t.v0.position.y, t.v0.position.z }, t.v0.normal, t.v0.u / t.v0.position.z, t.v0.v / t.v0.position.z},
		   	{ { t.v1.position.x, t.v1.position.y, t.v1.position.z }, t.v1.normal, t.v1.u / t.v1.position.z, t.v1.v / t.v1.position.z},
		   	{ { t.v2.position.x, t.v2.position.y, t.v2.position.z }, t.v2.normal, t.v2.u / t.v2.position.z, t.v2.v / t.v2.position.z}
	   	};
		return 1;
	}
	
	if (inCount == 1)
	{
		Vertex A = inside[0], B = outside[0], C = outside[1];	

		Vertex i1 = T_IntersectNear(A, B);
		Vertex i2 = T_IntersectNear(A, C);

		A.u /= A.position.z;
		A.v /= A.position.z;

		out[0] = (Triangle) { A, i1, i2 };

		return 1;
	}

	if (inCount == 2)
	{
		Vertex A = inside[0];
		Vertex B = inside[1];
		Vertex C = outside[0];

		Vertex a = A, b = B, c = C;
	
		Vertex i1 = T_IntersectNear(A, C);
		Vertex i2 = T_IntersectNear(B, C);

		A.u /= A.position.z;
		A.v /= A.position.z;
		B.u /= B.position.z;
		B.v /= B.position.z;

		out[0] = (Triangle) { A, B, i1 };
		out[1] = (Triangle) { B, i2, i1 };

		return 2;
	}

	return 0;
}

Vec3 ComputeTriangleNormal(Triangle t)
{
	Vec3 e1 = Vec3Sub(t.v1.position, t.v0.position);
	Vec3 e2 = Vec3Sub(t.v2.position, t.v0.position);
	return Vec3Normalize(Vec3Cross(e1, e2));
}

void PrintTriangle(Triangle t)
{
	Vec3 v0 = t.v0.position;
	Vec3 v1 = t.v1.position;
	Vec3 v2 = t.v2.position;
	printf("Triangle(v0={ { %f, %f, %f }, %f %f }, v1={ { %f, %f, %f }, %f %f }, v2={ { %f, %f, %f }, %f %f })", v0.x, v0.y, v0.z, t.v0.u, t.v0.v, v1.x, v1.y, v1.z, t.v1.u, t.v1.v, v2.x, v2.y, v2.z, t.v2.u, t.v2.v);
}
