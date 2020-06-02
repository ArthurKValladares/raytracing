#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include "texture.h"
#include "perlin.h"

class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		//return vec3(1, 1, 1) * noise.noise(scale * p);
		//return vec3(1, 1, 1) * 0.5 * (1 + noise.turbulence(scale * p));
		//return vec3(1, 1, 1) * noise.turbulence(scale * p);
		return vec3(1, 1, 1) * 0.5 * (1 + sin(scale*p.z() + 10*noise.turbulence(p)));
	}
	perlin noise;
	float scale;
};

#endif // !NOISE_TEXTURE_H

