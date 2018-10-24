#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitives/object.h"
#include "abstraction/types.h"
#include "math/vec.h"
#include "core/ray.h"
#include "core/aabb.h"
#include "primitives/vertex.h"
#include "primitives/trianglemesh.h"

class Triangle : public Object
{
	public:
	Vertex *a;
	Vertex *b;
	Vertex *c;

	vec normal;
	TriangleMesh *mesh;

	Triangle() : a(0), b(0), c(0) {}
	Triangle(const Vertex &av, const Vertex &bv, const Vertex &cv) : a((Vertex*)&av), b((Vertex*)&bv), c((Vertex*)&cv) {}
	Triangle(const Vertex &av, const Vertex &bv, const Vertex &cv, const TriangleMesh &m) : a((Vertex*)&av), b((Vertex*)&bv), c((Vertex*)&cv), mesh((TriangleMesh*)&m) {}

	bool intersect(Ray *ray, Intersection *isct)
	{
		//tihs is the Moeller and Trumbore, fast minimum storage ray triangle
		//intersection test
		vec edge1 = b->pos - a->pos;
		vec edge2 = c->pos - a->pos;
		vec pvec = ray->dir % edge2;

		real det = edge1 * pvec;
		if((det > (-EPSILON)) && (det < EPSILON)) return false;

		real inv_det = real(1) / det;
		vec tvec = ray->origin - a->pos;
		real u = (tvec * pvec) * inv_det;
		if((u < real(0)) || (u > real(1))) return false;

		vec qvec = tvec % edge1;
		real v = (ray->dir * qvec) * inv_det;
		if((v < real(0)) || ((u + v) > real(1))) return false;

		real dist = (edge2 * qvec) * inv_det;
		if((dist < ray->mint) || (dist > ray->maxt)) return false;

		isct->distance = dist;
		isct->hitpoint = ray->getPoint(dist);

		if(mesh->smoothed)
		{
			vec n = (a->normal * (real(1)-u-v)) + (b->normal * u) + (c->normal * v);
			n.normalize();
			isct->normal = n * real(-1);
			//isct->normal = normal;
		}
		else
		{
			isct->normal = normal;
		}
		

		return true;	
	}

	real calcTriNormal()
	{
		vec x = a->pos;
		vec y = b->pos;
		vec z = c->pos;
		normal = (z - x) % (y - x);
		return normal.normalize();
	}

	void getAABB(AABB *aabb)
	{
		aabb->add(a->pos);
		aabb->add(b->pos);
		aabb->add(c->pos);
	}

	Material* getMaterial()
	{
		return mesh->material;
	}
};

#endif
