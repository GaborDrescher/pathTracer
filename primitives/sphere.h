#ifndef SPHERE_H
#define SPHERE_H

#include "abstraction/types.h"
#include "math/vec.h"
#include "core/ray.h"
#include "core/aabb.h"

class Sphere : public Object
{
	public:
	vec mid;
	real radius;
	Material *material;

	Sphere(Material *mat) : radius(real(0)), material(mat) {}

	bool intersect(Ray *ray, Intersection *isct)
	{
		vec p = ray->origin - mid;

		real b = -(p * ray->dir);
		real det = b * b - (p * p) + radius * radius;

		if(det < real(0)) return false; //TODO EPSILON?

		det = sqrt(det);
		real i1 = b - det;
		real i2 = b + det;

		if(i2 < real(0)) return false; //TODO EPSILON?

		real outt = i2;
		if(i1 > real(0)) //TODO EPSILON?
		{
			outt = i1;
		}

		isct->distance = outt;
		isct->hitpoint = ray->origin + (ray->dir * outt);
		isct->normal = (isct->hitpoint - mid) / radius;

		return true;
	}

	void getAABB(AABB *aabb)
	{
		vec radvec(radius, radius, radius);
		aabb->add(mid+radvec);
		aabb->add(mid-radvec);
	}

	Material* getMaterial()
	{
		return material;
	}
};

#endif
