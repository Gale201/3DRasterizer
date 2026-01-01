#ifndef MESH_H
#define MESH_H

#include "math.h"

typedef struct {
	Vec3 *vertices;
	int vertexCount;

	int *indices;
	int indexCount;
} Mesh;

#endif
