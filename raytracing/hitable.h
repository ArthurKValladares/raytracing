#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "aabb.h"

// TODO: REDUNDANDE DEFINE CAMERA/SPHERE, SIMPLIFY LATER
#define PI 3.1415926535897

void get_sphere_uv(const vec3 & p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + PI) / (2 * PI);
	v = (theta + PI / 2) / (PI);
}

// We need to the compiler that material is a class 
// to avoid circularity of references between material and hitable.
class material;

struct hit_record
{
	// t is time hit occured, p is the point at which it occured,
	// normal is the normal to the surface, mat_ptr is a pointer to the material
	float t;
	float u;
	float v;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
};

// Abstract class hitable
class hitable
{
public:
	// Pure virtual function, must be overriden by sub-class
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	// Creates bounding box, takes t0 and t1 to specify time for moving objects.
	// return true if box created, false otherwise
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

#endif // !HITABLE_H

