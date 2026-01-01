#ifndef LOADER_H
#define LOADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mesh.h"

Mesh* LoadMeshFromObjFile(const char *path);

void DestroyMesh(Mesh* mesh);

#endif
