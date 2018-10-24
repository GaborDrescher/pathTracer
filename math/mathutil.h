#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "abstraction/types.h"
#include "config.h"
#include <math.h>

#ifdef M_PI
	#undef M_PI
#endif

#define M_PI real(3.1415926535897932384626)

class Math
{
	public:
	static bool approxZero(real x)
	{
		return (x < EPSILON) && (x > (-EPSILON));
	}

	static real abs(real x)
	{
		real out = (x < 0) ? (-x) : (x);
		
		if(out < 0) return real(0);

		return out;
	}

	static real min(real a, real b)
	{
		if(a < b) return a;
		return b;
	}

	static real max(real a, real b)
	{
		if(a > b) return a;
		return b;
	}

	static real sqrt(real x)
	{
		#if real == float
			return sqrtf(x);
		#elif real == double
			return sqrt(x);
		#else
			#error "no known sqrt"
		#endif
	}

	static real sin(real x)
	{
		#if real == float
			return sinf(x);
		#elif real == double
			return sin(x);
		#else
			#error "no known sin"
		#endif
	}

	static real cos(real x)
	{
		#if real == float
			return cosf(x);
		#elif real == double
			return cos(x);
		#else
			#error "no known cos"
		#endif
	}

	static real exp(real x)
	{
		#if real == float
			return expf(x);
		#elif real == double
			return exp(x);
		#else
			#error "no known exp"
		#endif
	}

	static real pow(real x, real y)
	{
		#if real == float
			return powf(x, y);
		#elif real == double
			return pow(x,y);
		#else
			#error "no known pow"
		#endif
	}

	template<typename T>
	static T clamp(T val, T low, T high) {
		if (val < low) return low;
		else if (val > high) return high;
		else return val;
	}


	static void ConcentricSampleDisk(real u1, real u2, real *dx, real *dy) {
        real r, theta;
        // Map uniform random numbers to [-1,1]^2
        real sx = real(2) * u1 - real(1);
        real sy = real(2) * u2 - real(1);

        // Map square to (r,\theta)

        // Handle degeneracy at the origin
        if (sx == real(0) && sy == real(0)) {
            *dx = real(0);
            *dy = real(0);
            return;
        }
        if (sx >= -sy) {
            if (sx > sy) {
                // Handle first region of disk
                r = sx;
                if (sy > real(0)) theta = sy/r;
                else          theta = real(8) + sy/r;
            }
            else {
                // Handle second region of disk
                r = sy;
                theta = real(2) - sx/r;
            }
        }
        else {
            if (sx <= sy) {
                // Handle third region of disk
                r = -sx;
                theta = real(4) - sy/r;
            }
            else {
                // Handle fourth region of disk
                r = -sy;
                theta = real(6) + sx/r;
            }
        }
        theta *= real(M_PI) / real(4);
        *dx = r * Math::cos(theta);
        *dy = r * Math::sin(theta);
    }

};

#endif
