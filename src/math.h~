#ifndef MATH_H
#define MATH_H

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vec4;

typedef struct {
	Vec3 position;
} Vertex;

typedef struct {
	Vec3 v0;
	Vec3 v1;
	Vec3 v2;
} Triangle;

typedef struct {
	float m[4][4];
} Mat4;	

// Vector operations
Vec3 Vec3Add(Vec3 a, Vec3 b);
Vec3 Vec3Opposite(Vec3 a);
Vec3 Vec3Sub(Vec3 a, Vec3 b);
Vec3 Vec3Cross(Vec3 a, Vec3 b);
float Vec3Dot(Vec3 a, Vec3 b);
Vec3 Vec3Normalize(Vec3 v);
Vec3 Vec3Scale(Vec3 v, float s);

// Vector transforms
Vec3 Vec3Translate(Vec3 v, Vec3 t);
Vec3 Vec3RotateZ(Vec3 v, float angle);
Vec3 Vec3RotateX(Vec3 v, float angle);
Vec3 Vec3RotateY(Vec3 v, float angle);

Vec3 Vec3RotateZAroundPoint(Vec3 v, Vec3 p, float angle);
Vec3 Vec3RotateXAroundPoint(Vec3 v, Vec3 p, float angle);
Vec3 Vec3RotateYAroundPoint(Vec3 v, Vec3 p, float angle);

// Matrix operations
Mat4 Mat4Identity(void);
Mat4 Mat4Mul(Mat4 a, Mat4 b);
Vec4 Mat4MulVec4(Mat4 m, Vec4 v);

// Matrix transforms
Mat4 Mat4Translate(float x, float y, float z);
Mat4 Mat4RotateX(float angle);
Mat4 Mat4RotateY(float angle);
Mat4 Mat4RotateZ(float angle);
Mat4 Mat4Scale(float s);
Mat4 Mat4ScaleVec3(Vec3 v);

void ProjectVertex(Vec3 v, int *x, int *y, float *z);

#endif
