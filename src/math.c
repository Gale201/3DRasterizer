#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "math.h"
#include "renderer.h"

static const float fov = 1.f;

// Basic operations

void swap_int(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void swap_float(float *a, float *b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}

static inline float FastInvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

// Vector operations

Vec3 Vec3Add(Vec3 a, Vec3 b)
{
	return (Vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 Vec3Opposite(Vec3 a)
{
	return (Vec3) { -a.x, -a.y, -a.z };
}

Vec3 Vec3Sub(Vec3 a, Vec3 b)
{
	return Vec3Add(a, Vec3Opposite(b));
}

Vec3 Vec3Cross(Vec3 a, Vec3 b)
{
	return (Vec3) { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

float Vec3Dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3Normalize(Vec3 v)
{
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	//float length = FastInvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return (Vec3) { v.x / length, v.y / length, v.z / length };
}

Vec3 Vec3Scale(Vec3 v, float s)
{
	return (Vec3) { v.x * s, v.y * s, v.z * s };
}

// Vec3 transforms

Vec3 Vec3Translate(Vec3 v, Vec3 t)
{
	return (Vec3) { v.x + t.x, v.y + t.y, v.z + t.z };
}

Vec3 Vec3RotateZ(Vec3 v, float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return (Vec3) { v.x * c - v.y * s, v.x * s + v.y * c, v.z };
}

Vec3 Vec3RotateX(Vec3 v, float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return (Vec3) { v.x, v.y * c - v.z * s, v.y * s + v.z * c };
}

Vec3 Vec3RotateY(Vec3 v, float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return (Vec3) { v.x * c + v.z * s, v.y, -v.x * s + v.z * c };
}

Vec3 Vec3RotateZAroundPoint(Vec3 v, Vec3 point, float angle)
{
	v = Vec3Translate(v, Vec3Opposite(point));
	v = Vec3RotateZ(v, angle);
	v = Vec3Translate(v, point);

	return v;
}

Vec3 Vec3RotateXAroundPoint(Vec3 v, Vec3 point, float angle)
{
	v = Vec3Translate(v, Vec3Opposite(point));
	v = Vec3RotateX(v, angle);
	v = Vec3Translate(v, point);

	return v;
}
Vec3 Vec3RotateYAroundPoint(Vec3 v, Vec3 point, float angle)
{
	v = Vec3Translate(v, Vec3Opposite(point));
	v = Vec3RotateY(v, angle);
	v = Vec3Translate(v, point);

	return v;
}

// Matrix operations
Mat4 Mat4Identity(void)
{
	Mat4 m = {0};	
	m.m[0][0] = 1.0f;
	m.m[1][1] = 1.0f;
	m.m[2][2] = 1.0f;
	m.m[3][3] = 1.0f;
	return m;
}

Mat4 Mat4Mul(Mat4 a, Mat4 b)
{
	Mat4 r = {0};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				r.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return r;
}

Vec4 Mat4MulVec4(Mat4 m, Vec4 v)
{
	Vec4 r;
	r.x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3];
	r.y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3];
	r.z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3];
	r.w = v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3];
	return r;
}

// Matrix transforms
Mat4 Mat4Translate(float x, float y, float z)
{
	Mat4 m = Mat4Identity();
	m.m[0][3] = x;
	m.m[1][3] = y;
	m.m[2][3] = z;
	return m;
}

Mat4 Mat4RotateX(float angle)
{
	Mat4 m = Mat4Identity();
	float c = cosf(angle);
	float s = sinf(angle);

	m.m[1][1] = c;
	m.m[1][2] = -s;
	m.m[2][1] = s;
	m.m[2][2] = c;
	return m;
}

Mat4 Mat4RotateY(float angle)
{	
	Mat4 m = Mat4Identity();
	float c = cosf(angle);
	float s = sinf(angle);

	m.m[0][0] = c;
	m.m[0][2] = s;
	m.m[2][0] = -s;
	m.m[2][2] = c;
	return m;
}

Mat4 Mat4RotateZ(float angle)
{	
	Mat4 m = Mat4Identity();
	float c = cosf(angle);
	float s = sinf(angle);

	m.m[0][0] = c;
	m.m[0][1] = -s;
	m.m[1][0] = s;
	m.m[1][1] = c;
	return m;
}

Mat4 Mat4Scale(float s)
{
	Mat4 m = Mat4Identity();
	m.m[0][0] = s;
	m.m[1][1] = s;
	m.m[2][2] = s;
	return m;
}

Mat4 Mat4ScaleVec3(Vec3 v)
{
	Mat4 m = Mat4Identity();
	m.m[0][0] = v.x;
	m.m[1][1] = v.y;
	m.m[2][2] = v.z;
	return m;
}

// Projection

Mat4 Mat4Perspective(float zNear, float zFar)
{
	float f = 1.0f / tanf(fov * 0.5f);

	Mat4 m = {0};

	float aspect = (float) RendererGetWidth() / (float) RendererGetHeight();
	
	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = (zFar + zNear) / (zNear - zFar);
	m.m[2][3] = (2.0f * zFar * zNear) / (zNear - zFar);
	m.m[3][2] = -1.0f;
	m.m[3][3] = 0.0f;

	return m;
}

Vertex ProjectVertex(Vertex *v)
{
	float aspect = (float) RendererGetWidth() / (float) RendererGetHeight();
	float px = v->position.x / v->position.z / aspect;
	float py = v->position.y / v->position.z;

	Vertex ret;

	ret.position.x = (px + 1.0f) * 0.5f * RendererGetWidth();
	ret.position.y = (1.0f - py) * 0.5f * RendererGetHeight();
	ret.position.z = v->position.z;
	ret.u = v->u;
	ret.v = v->v;
	ret.normal = v->normal;
	return ret;
}

