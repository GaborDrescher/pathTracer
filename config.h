#ifndef CONFIG_H
#define CONFIG_H

#include "abstraction/types.h"

//epsilon depending on float type
	#if real == float
		#define EPSILON (real(1e-5))
	#elif real == double
		#define EPSILON (real(1e-9))
	#else
		#error "no known epsilon"
	#endif
////

//max and min values
	#if real == float
		#define REAL_MAX (real(1e20))
		#define REAL_MIN (real(-1e20))
	#elif real == double
		#define REAL_MAX (real(1e100))
		#define REAL_MIN (real(-1e100))
	#else
		#error "no known min/max for real type"
	#endif
////

#endif

