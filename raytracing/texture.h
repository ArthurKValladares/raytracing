#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"

class texture {
public:
	// Returns rgb value at width u, height v.
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

#endif
