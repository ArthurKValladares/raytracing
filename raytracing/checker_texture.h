#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "texture.h"

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(texture* t0, texture* t1) : even(t0), odd(t1) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		// Since the sign of sin fluactuates between negative and positive at a regular interval, 
		// multplying the sins of N dimensions will give us an N dimensional checkerboard pattern.
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0) {
			return odd->value(u, v, p);
		} else {
			return even->value(u, v, p);
		}
	}
	texture* even;
	texture* odd;
};

#endif // !CHECKER_TEXTURE_H

