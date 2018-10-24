#ifndef TRACER_RANDOM_HEADER
#define TRACER_RANDOM_HEADER

#include "abstraction/types.h"
#include "math/vec.h"

class Random
{
	public:
	static void init(uint64_t seed);
	static real getSample1D();
	static vec getSample2D();
};

#endif /* TRACER_RANDOM_HEADER */
