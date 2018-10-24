#ifndef VEC_H
#define VEC_H

#include "math/real.h"
#include "math/mathutil.h"

class vec
{
	public:
	union
	{
		struct
		{
			real x;
			real y;
			real z;
			#ifdef VEC_PADDING
			real w;
			#endif
		};

		#ifdef VEC_PADDING
		real data[4];
		#else
		real data[3];
		#endif
	};

	vec() : x(real(0)), y(real(0)), z(real(0)) {}
	vec(real x_, real y_, real z_) : x(x_), y(y_), z(z_) {}

	vec operator+(const vec other) const
	{
		return vec(x+other.x, y+other.y, z+other.z);
	}

	vec operator-(const vec other) const
	{
		return vec(x-other.x, y-other.y, z-other.z);
	}

	vec operator*(const real other) const
	{
		return vec(x*other, y*other, z*other);
	}

	real operator*(const vec other) const
	{
		return x*other.x + y*other.y + z*other.z;
	}

	vec mult(const vec other) const
	{
		return vec(x*other.x, y*other.y, z*other.z);
	}

	vec operator%(const vec other) const
	{
		return vec(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
	}

	vec operator/(real other) const
	{
		if(Math::approxZero(other))
		{
			other = EPSILON;
		}

		const real scale = real(1) / other;
		return (*this) * scale;
	}

	real lengthSq() const
	{
		return x*x + y*y + z*z;
	}

	real length() const
	{
		return Math::sqrt(lengthSq());
	}

	real normalize()
	{
		real len = length();
		*this = (*this)/len;
		return len;
	}

	vec getNormal(real *le) const
	{
		real len = length();
		*le = len;
		return (*this)/len;
	}

	vec getNormal() const
	{
		real len = length();
		return (*this)/len;
	}

    static void reflect(const vec &wo, vec &wi)
	{
		wi = vec(-wo.x, -wo.y, wo.z);
	}

    static void cosineSampleHemisphere(const real u1, const real u2, vec& v) {
        Math::ConcentricSampleDisk(u1, u2, &v.x, &v.y);
        v.z = Math::sqrt(Math::max(real(0), real(1) - v.x * v.x - v.y * v.y));
    }

    static void uniformSampleHemisphere(const real u1, const real u2, vec& v) {
        real z = u1;
        real r = Math::sqrt(Math::max(real(0), real(1) - z*z));
        real phi = real(2) * M_PI * u2;
        real x = r * Math::cos(phi);
        real y = r * Math::sin(phi);
        v.x = x;
        v.y = y;
        v.z = z;
    }

    static vec UniformSampleSphere(real u1, real u2) {
        real z = real(1) - real(2) * u1;
        real r = Math::sqrt(Math::max(real(0), real(1) - z*z));
        real phi = real(2) * M_PI * u2;
        real x = r * Math::cos(phi);
        real y = r * Math::sin(phi);
        return vec(x, y, z);
    }
	real operator[](unsigned int idx)
	{
		return data[idx];
	}
};

#endif
