#ifndef MIRROR_MATERIAL_H
#define MIRROR_MATERIAL_H

#include "materials/material.h"
#include "math/mathutil.h"
#include "math/vec.h"

class MirrorMaterial : public Material
{
	private:
	Color color;

	public:
	MirrorMaterial(Color col) : color(col) {}

	void sample(Query &query)
	{
		query.outdir = query.dir - query.normal * (real(2) * (query.normal * query.dir));
		query.type = REFLECTIVE;
		query.color = color;
	}

	Color getColor()
	{
		return color;
	}
};

#endif
