#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hitable.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const { return false; }
	virtual vec3 emitted(float u, float v, const vec3& p) const {
		return vec3(0, 0, 0);
	}
};

#endif
