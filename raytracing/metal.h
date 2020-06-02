#ifndef METAL_H
#define METAL_H

class metal : public material
{
public:
	metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		// reflect vector in relation to surface normal.
		// Then, add a new point in a unit sphere around the reflected point, and scale it by fuzz
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		// create ray at hit position with reflected direction
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
		// set attenuation/how much light is reflected
		attenuation = albedo;
		// Dot product would onlt be 0 if one of the vectors was zero,
		// or reflected and normal vectors are perpendicular.
		// in that case, no scattering should happen, as the ray does't actually hit the object.
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	// Albedo is the proportion of incident light that is reflected by surface
	vec3 albedo;
	float fuzz;
};

#endif // !METAL_H


