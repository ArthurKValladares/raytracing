#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"
#include "ray.h"

// Code for Schlick's approximation, approximates
// the Fresnel factor in the specular reflection of light from a non-conducting (dielectric) surface between two media.
// with n1 = 1, simulating air
float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class dielectric : public material
{
public:
	dielectric(const vec3& a, float ri) : albedo(a), ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		vec3 refracted;
		attenuation = albedo;
		float ni_over_nt;
		float reflect_prob;
		float cosine;
		// angle between in vector and surface normal lesser than 90.
		// since normal points from center to hit point, this means normal aray both point in same
		// v direction, so the ray has to be leaving the surface. 
		if (dot(r_in.direction(), rec.normal) > 0)
		{
			// We invert the normal, since it has to point inwards.
			outward_normal = -rec.normal;
			// ni_over_nt will just be the ref_idx, as air (n_2) has index 1
			ni_over_nt = ref_idx;
			cosine = dot(unit_vector(r_in.direction()), rec.normal);
		}
		// case where ray is going into surface
		else
		{
			outward_normal = rec.normal;
			// invert ref_idx, since air (n_1) has index 1
			ni_over_nt = 1.0/ref_idx;
			// Since in vector and normal are going in different directions,
			// we need to invert the cosine to get the actual angle of the incident light
			cosine = -dot(unit_vector(r_in.direction()), rec.normal);
		}

		// If refracted ray exists, get reflectiong probability according to schlicks.
		// otherwise, always reflect (prop 1)
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			reflect_prob = 1.0;
		}
		if (random_double() < reflect_prob)
		{
			scattered = ray(rec.p, reflected, r_in.time());
		}
		else
		{
			scattered = ray(rec.p, refracted, r_in.time());
		}
		return true;
	}
	float ref_idx;
	vec3 albedo;
};

#endif // !DIELECTRIC_H

