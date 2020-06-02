#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "texture.h"
#include "material.h"
#define _USE_MATH_DEFINES
#include <math.h>

class lambertian : public material
{
public:
	lambertian(texture *a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {

		// we are bouncing the light randomly inside a unit sphere tangent to the hit point
		// Center of sphere is (rec.p + rec.normal) i.e, exactly one unit straight "above" the hit point
		// then, we calculate a point s inside a generic unit sphere at (0, 0, 0), 
		// and add that point to our sphere center to get the equivalent point in our sphere.
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		// create ray at hit position with reflected direction
		scattered = ray(rec.p, target - rec.p, r_in.time());
		// set attenuation/how much light is reflected
		// equal to a new texture at the point it was hit.
		// First two parameters irrelavant for const_texture in lambertian
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
	float scattering_pdf(const ray& r_in, const hit_record& rec, ray& scattered) const {
		float cosine = dot(rec.normal, unit_vector(scattered.direction()));
		if (cosine < 0) cosine = 0;
		return cosine / M_PI;
	}
	// Albedo is the proportion of incident light that is reflected by surface
	// determined by colors in the texture
	texture *albedo;
};

#endif // !LAMBERTIAN_H

