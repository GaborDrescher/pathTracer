#include "primitives/trianglemesh.h"
#include "primitives/triangle.h"

void TriangleMesh::init()
{
	for(size_t i = 0; i != ntris; ++i)
	{
		triangles[i].mesh = this;
		const real len = triangles[i].calcTriNormal();
		triangles[i].a->normal = triangles[i].a->normal + (triangles[i].normal * len);
		triangles[i].b->normal = triangles[i].b->normal + (triangles[i].normal * len);
		triangles[i].c->normal = triangles[i].c->normal + (triangles[i].normal * len);
		triangles[i].a->tmp += len;
		triangles[i].b->tmp += len;
		triangles[i].c->tmp += len;
	}
	for(size_t i = 0; i != nverts; ++i)
	{
		vec n = vertices[i].normal;
		//vertices[i].normal = vertices[i].normal / vertices[i].tmp;
		//vertices[i].normal.normalize();

		n.normalize();
		vertices[i].normal = n;
		vertices[i].tmp = real(0);
	}
}
