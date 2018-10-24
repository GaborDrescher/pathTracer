#ifndef DIFFUSE_MATERIAL_H
#define DIFFUSE_MATERIAL_H

#include "materials/material.h"
#include "math/mathutil.h"
#include "math/vec.h"
#include "math/onb.h"
#include "core/random.h"

class DiffuseMaterial : public Material
{
	private:
	Color color;

	public:
	DiffuseMaterial(Color col) : color(col) {}

	void sample(Query &query)
	{
		vec nl = query.normal * (query.dir) < 0 ? query.normal : query.normal*real(-1);

        ONB onb(nl);
        vec outdir;
        vec::cosineSampleHemisphere(Random::getSample1D(), Random::getSample1D(), outdir);

        outdir = onb.toWorld(outdir);
        outdir.normalize();
		query.outdir = outdir;
		query.type = DIFFUSE;
		query.color = color;
	}

	Color getColor()
	{
		return color;
	}
};

#endif
