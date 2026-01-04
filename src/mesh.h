#ifndef MESH_H
#define MESH_H

#include "math.h"

typedef struct {
	Vertex *vertices;
	int vertexCount;

	int *indices;
	int indexCount;
} Mesh;

#endif
