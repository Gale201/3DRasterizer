#include "loader.h"

static int L_realloc_Vec2(Vec2** array, int* size)
{
	*size = (*size) * 2;
	*array = (Vec2*) realloc(*array, (*size) * sizeof(Vec2));
	if (*array == NULL)
	{
		printf("Memory realloc failed!\n");
		return 0;
	}
	return 1;
}

static int L_realloc_Vec3(Vec3** array, int* size)
{
	*size = (*size) * 2;
	*array = (Vec3*) realloc(*array, (*size) * sizeof(Vec3));
	if (*array == NULL)
	{
		printf("Memory realloc failed!\n");
		return 0;
	}
	return 1;
}

static int L_realloc_ObjIndex(ObjIndex** array, int* size)
{
	*size = (*size) * 2;
	*array = (ObjIndex*) realloc(*array, (*size) * sizeof(ObjIndex));
	if (*array == NULL)
	{
		printf("Memory realloc failed!\n");
		return 0;
	}
	return 1;
}

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
	ObjIndex *indices = (ObjIndex*) malloc(defaultSize * sizeof(ObjIndex));
	Vec2 *textureCoordinates = (Vec2*) malloc(defaultSize * sizeof(Vec2));
	Vec3 *normals = (Vec3*) malloc(defaultSize * sizeof(Vec3));

	if (vertices == NULL || indices == NULL || textureCoordinates == NULL || normals == NULL)
	{
		printf("Memory allocation failed!\n");
		return mesh;
	}

	int vIdx = 0;
	int iIdx = 0;
	int uvIdx = 0;
	int nIdx = 0;

	int vSize = defaultSize;
	int iSize = defaultSize;
	int uvSize = defaultSize;
	int nSize = defaultSize;

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		int vertex[4], texture[4], normal[4];
		
		// Parsing vertices
		if (line[0] == 'v' && line[1] == ' ' && (sscanf(line, "v  %f %f %f", &x, &y, &z) == 3 || sscanf(line, "v %f %f %f", &x, &y, &z) == 3))
		{
			if (vIdx + 1 >= vSize && !L_realloc_Vec3(&vertices, &vSize)) return mesh;
			
			vertices[vIdx++] = (Vec3) { x, y, z };
		}
		// Parsing quad faces
		else if
			(line[0] == 'f' &&
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
			if (iIdx + 6 >= iSize && !L_realloc_ObjIndex(&indices, &iSize)) return mesh;

			indices[iIdx].pos = vertex[0];
			indices[iIdx].uv = texture[0];
			indices[iIdx++].normal = normal[0];

			indices[iIdx].pos = vertex[1];
			indices[iIdx].uv = texture[1];
			indices[iIdx++].normal = normal[1];
			
			indices[iIdx].pos = vertex[2];
			indices[iIdx].uv = texture[2];
			indices[iIdx++].normal = normal[2];

			indices[iIdx].pos = vertex[0];
			indices[iIdx].uv = texture[0];
			indices[iIdx++].normal = normal[0];

			indices[iIdx].pos = vertex[2];
			indices[iIdx].uv = texture[2];
			indices[iIdx++].normal = normal[2];

			indices[iIdx].pos = vertex[3];
			indices[iIdx].uv = texture[3];
			indices[iIdx++].normal = normal[3];
		}
		// Parsing triangle faces with normals
		else if (line[0] == 'f' && sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex[0], &texture[0], &normal[0], &vertex[1], &texture[1], &normal[1], &vertex[2], &texture[2], &normal[2]) == 9)
		{
			if (iIdx + 3 >= iSize && !L_realloc_ObjIndex(&indices, &iSize)) return mesh;

			indices[iIdx].pos = vertex[0];
			indices[iIdx].uv = texture[0];
			indices[iIdx++].normal = normal[0];

			indices[iIdx].pos = vertex[1];
			indices[iIdx].uv = texture[1];
			indices[iIdx++].normal = normal[1];

			indices[iIdx].pos = vertex[2];
			indices[iIdx].uv = texture[2];
			indices[iIdx++].normal = normal[2];
		}
		// Parsing triangle faces without normals
		else if (line[0] == 'f' && sscanf(line, "f %d/%d %d/%d %d/%d", &vertex[0], &texture[0], &vertex[1], &texture[1], &vertex[2], &texture[2]) == 6)
		{
			if (iIdx + 3 >= iSize && !L_realloc_ObjIndex(&indices, &iSize)) return mesh;

			indices[iIdx].pos = vertex[0];
			indices[iIdx++].uv = texture[0];

			indices[iIdx].pos = vertex[1];
			indices[iIdx++].uv = texture[1];

			indices[iIdx].pos = vertex[2];
			indices[iIdx++].uv = texture[2];
		}
		// Parsing texture coordinates
		else if (line[0] == 'v' && line[1] == 't' && sscanf(line, "vt %f %f", &u, &v) == 2)
		{
			if (uvIdx + 1 >= uvSize && !L_realloc_Vec2(&textureCoordinates, &uvSize)) return mesh;

			textureCoordinates[uvIdx++] = (Vec2) { u, v };
		}
		// Parsing vertex normals
		else if (line[0] == 'v' && line[1] == 'n' && sscanf(line, "vn %f %f %f", &nx, &ny, &nz) == 3)
		{
			if (nIdx + 1 >= nSize && !L_realloc_Vec3(&normals, &nSize)) return mesh;

			normals[nIdx++] = (Vec3) { nx, ny, nz };
		}
	}

	Vertex* finalVertices = (Vertex*) malloc(iIdx * sizeof(Vertex));
	int* finalIndices = (int*) malloc(iIdx * sizeof(int));

	for (int i = 0; i < iIdx; i++)
	{
		ObjIndex oIdx = indices[i];

		Vertex v;
		v.position = vertices[oIdx.pos - 1];
		v.u = textureCoordinates[oIdx.uv - 1].x;
		v.v = 1.0f - textureCoordinates[oIdx.uv - 1].y;
		v.normal = normals[oIdx.normal - 1];

		finalVertices[i] = v;
		finalIndices[i] = i;
	}

	mesh->vertices = finalVertices;
	mesh->vertexCount = iIdx;
	mesh->indices = finalIndices;
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
