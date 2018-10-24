#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vec.h"
#include "core/color.h"

enum MatType
{
	DIFFUSE,
	REFLECTIVE,
	TRANSMISSIVE
};

class Query
{
	public:
	//in
	vec origin;
	vec dir;
	vec hitpoint;
	vec normal;
	real distance;

	//out
	vec outdir;
	MatType type;
	Color color;
};

class Material
{
	public:
	virtual void sample(Query &query) = 0;
	virtual Color getColor() = 0;
};

#endif
