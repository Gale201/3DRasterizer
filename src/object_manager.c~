#include "object_manager.h"

#define RO_SIZE 100

static RObject *renderObjects = NULL;
static int robjsCount = 0;
static int robjsSize = RO_SIZE;

void ObjectManagerInit()
{
	renderObjects = (RObject*) malloc(RO_SIZE * sizeof(RObject));
	if (renderObjects == NULL)
	{
		printf("Malloc failed!\n");
		return;
	}
}

void AddRenderObject(RObject o)
{
	if (robjsCount >= robjsSize)
	{
		robjsSize *= 2;
		renderObjects = (RObject*) realloc(renderObjects, robjsSize * sizeof(RObject));
		if (renderObjects == NULL)
		{
			printf("Realloc failed!\n");
			return;
		}
	}
	renderObjects[robjsCount++] = o;
}

RObject CreateRenderObjectFromMesh(Mesh* mesh, Mat4 transform, uint32_t color)
{
	return (RObject) { mesh, transform, color };
}

void ObjectManagerRender()
{
	for (int i = 0; i < robjsCount; i++)
	{
		RenderObject(&renderObjects[i]);
	}
}

void ObjectManagerDestroy()
{
	for (int i = 0; i < robjsCount; i++)
	{
		DestroyMesh(renderObjects[i].mesh);
	}
}

RObject* GetRenderObjectByIndex(int index)
{
	return &renderObjects[index];
}
