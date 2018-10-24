#include "core/random.h"
#include "abstraction/types.h"
#include "math/vec.h"

static uint64_t s[16];
static int p;


static uint64_t nextRand()
{ 
	uint64_t s0 = s[p];
	uint64_t s1 = s[p = (p + 1) & 15];
	s1 ^= s1 << 31;
	s1 ^= s1 >> 11;
	s0 ^= s0 >> 30;
	return (s[p] = s0 ^ s1) * 1181783497276652981LL;
}

void Random::init(uint64_t seed)
{
	uint64_t x = seed;
	if(x == 0) {
		x = 42;
	}
	
	for(int i = 0; i < 16; ++i) {
		x ^= x >> 12;
		x ^= x << 25;
		x ^= x >> 27;
		s[i] = x * 2685821657736338717LL;
	}
	p = 0;
	
	for(int i = 0; i < 16; ++i) {
		nextRand();
	}
}

real Random::getSample1D()
{
	real out = real((uint32_t)(nextRand())) / real(4294967296.0);
	return out;
}

vec Random::getSample2D()
{
	return vec(getSample1D(), getSample1D(), 0);
}
