#include "loader.h"

Mesh* LoadMeshFromObjFile(const char *path)
{
	FILE *file;
	file = fopen(path, "r");

	Mesh *mesh = (Mesh*) malloc(sizeof(Mesh));

	if (file == NULL)
	{
		printf("Failed to open file: %s\n", path);
		return mesh;
	}

	const int defaultSize = 32;

	Vec3 *vertices = (Vec3*) malloc(defaultSize * sizeof(Vec3));
	int *indices = (int*) malloc(defaultSize * sizeof(int));

	if (vertices == NULL || indices == NULL)
	{
		printf("Memory allocation failed!\n");
		return mesh;
	}

	int vIdx = 0;
	int iIdx = 0;

	int vSize = defaultSize;
	int iSize = defaultSize;

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		float x = 0, y = 0, z = 0;
		int vertex[4], texture[4], normal[4];
		
		if (line[0] == 'v' && (sscanf(line, "v  %f %f %f", &x, &y, &z) == 3 || sscanf(line, "v %f %f %f", &x, &y, &z) == 3))
		{
			if (vIdx + 1 >= vSize)
			{
				vSize *= 2;
				vertices = (Vec3*) realloc(vertices, vSize * sizeof(Vec3));
				if (vertices == NULL)
				{
					printf("Memory realloc failed!\n");
					return mesh;
				}
			}
			vertices[vIdx++] = (Vec3) { x, y, z };
		}
		else if
		   	(line[0] = 'f' &&
			   	sscanf(
					line,
				   	"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				   	&vertex[0], &texture[0], &normal[0],
				   	&vertex[1], &texture[1], &normal[1],
				   	&vertex[2], &texture[2], &normal[2],
				   	&vertex[3], &texture[3], &normal[3]
				) == 12
			)
		{
			if (iIdx + 6 >= iSize)
			{
				iSize *= 2;
				indices = (int*) realloc(indices, iSize * sizeof(int));
				if (indices == NULL)
				{
					printf("Memory realloc failed!\n");
					return mesh;
				}
			}
			indices[iIdx++] = vertex[0] - 1;
			indices[iIdx++] = vertex[1] - 1;
			indices[iIdx++] = vertex[2] - 1;
			
			indices[iIdx++] = vertex[0] - 1;
			indices[iIdx++] = vertex[2] - 1;
			indices[iIdx++] = vertex[3] - 1;
		}
	}

	mesh->vertices = vertices;
	mesh->vertexCount = vIdx;
	mesh->indices = indices;
	mesh->indexCount = iIdx;

	fclose(file);

	return mesh;
}

void DestroyMesh(Mesh* mesh)
{
	free(mesh->vertices);
	free(mesh->indices);
	free(mesh);
}
