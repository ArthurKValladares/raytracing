#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class sphere : public hitable
{
public:
	sphere() {}
	sphere(const vec3& c, float r, material *mat) :center(c), radius(r), mat_ptr(mat) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center;
	float radius;
	material* mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	// Point is in sphere if it satifies:
	// (x-c_x)^2 + (y-c_y)^2 + (z-c_z)^2 = R^2
	// Where:
	// (x,y,z) are the coordinates for the point
	// (c_x, c_y, c_z) are the coordinates for the center of the sphere
	// R is the radius of the sphere.
	// Then we can see that this is equivalent to a dot procuct on the vector below with itself
	// ((x-c_x), (y-c_y), (z-c_z))
	// This vector is equivalent to the vector of the point - the vector of the center, so we have
	// dot((p-c), (p-c)) = R^2
	// Then substituting out point, which we get from the origin and direction of ray, scaled by (t)
	// dot((p(t)-c), (p(t)-c)) = R^2
	// dot((A+t*B-c), (A+t*B-c)) = R^2
	// expanding this we get:
	// t*t*dot(B,B) + 2*t*dot(B, A-C) + dot(A-C, A-C) - R*R = 0
	// Since this is a quadratic equation in t, we can check the root inse the quadratic equation, and if we get a number:
	// (d < 0) - No Solution, No intersections
	// (d = 0) - 1 Solution, 1 intersection
	// (d > 0) - 2 solutions, 2 intersections

	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0)
	{
		// t value for intersection point
		float t = (-b - sqrt(discriminant)) / (2.0*a);
		if (t < t_max && t > t_min)
		{
			// Valid hit, save hit information in rec
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			// normal vector for a sphere is in direction of hitpoint - center
			// normalized to make shading easier.
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center)/radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		// If (- discriminant) doesn't work, test second solution with (+ discriminant)
		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t < t_max && t > t_min)
		{
			// Valid hit, save hit information in rec
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			// normal vector for a sphere is in direction of hitpoint - center
			// normalized to make shading easier.
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	// If neither works, no valid hit
	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
	// creates a bounding box around sphere, return true.
	// Simply a cube surrounding the sphere.
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}
#endif // !SPHERE_H