#include "math/vec.h"

class ONB
{
	public:
	vec n;
	vec s;
	vec t;

	ONB(const vec &n, const vec &s, const vec &t) :n(n), s(s), t(t) {}

	ONB(const vec &vector) :n (vector)
	{	
		if(Math::abs(n.x) > Math::abs(n.z))
		{
			s.x = -n.y;
			s.y =  n.x;
			s.z =  0;
		}
		else
		{
			s.x =  0;
			s.y = -n.z;
			s.z =  n.y;
		}

		s.normalize();
		t = s % n;
	}

	// transform world coordinate system vector to onb coordinate system
	vec toONB(const vec &v) const 
	{
		return vec(v*s, v*t, v*n);
	}

	// transform onb local vector to world coordinate system
	vec toWorld(const vec &v) const
	{
		return (s * v.x) + (t * v.y) + (n * v.z);			
	}

	// Return cosine of angle between normal and vector for vector given in local coordinate system 
	real cosTheta(const vec &v) const
	{
		return v.z;
	}

	// Return squared sine of angle between normal and given vector in local coordinate system
	real sinTheta2(const vec &v) const
	{
		return real(1) - v.z * v.z;
	}
};
