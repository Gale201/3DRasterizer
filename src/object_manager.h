#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdio.h>

#include "renderer.h"
#include "mesh.h"
#include "loader.h"

void ObjectManagerInit();

void AddRenderObject(RObject o);

RObject CreateRenderObjectFromMesh(Mesh* mesh, Mat4 transform, uint32_t color);

void ObjectManagerRender();

void ObjectManagerDestroy(); 

RObject* GetRenderObjectByIndex(int index);

#endif
