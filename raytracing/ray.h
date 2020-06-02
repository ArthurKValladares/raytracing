#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// COMMENT THIS
class ray
{
public:
	ray() {}
	ray(const vec3& a, const vec3& b, float ti = 0.0) : A(a), B(b), _time(ti){}
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t) const { return A + t*B; }
	float time() const { return _time; }

	// Origin
	vec3 A;
	// Direction vector
	vec3 B;
	// Time at which the ray exists, used for motion blur
	float _time;
};

#endif // !RAY_H

