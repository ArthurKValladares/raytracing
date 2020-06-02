#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "vec3.h"

// TODO: ADD SLIGHT PADDING TO HANDLE GRACE CASES

// faster min and max function, doesn't worry about NaN or other exceptions
inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
	friend aabb surrounding_box(aabb box0, aabb box1);
public:
	aabb() {}
	// Create axis-aligned bounding box with 'slab' method.
	// treats aabb as the intersection of 3 'slabs', or axis-aligned intervals.
	// Each interval is just the points between two endpoints, e.g. x in [3,5] or z in [-2, 7]
	// Pass minimum endpoints in a, maximum endpoints in b
	aabb(const vec3& a, const vec3& b): _min(a), _max(b) {}
	vec3 min() const { return _min; }
	vec3 max() const { return _max; }
	// With an aabb box, we have a hit when the timeframe for all 3 axis have an overlap.
	bool hit(const ray& r, float tmin, float tmax) const {
		// test for all 3 dimentions.
		for (int i = 0; i < 3; ++i) {
			// The formula for a ray is p(t) = A + t*B.
			// Then, the formula for a specific coordinate of the ray is:
			// x(t) = A.x() + t*B.x()
			// For a ray, A = origin, B = direction
			// Then to check the time at which the ray hits x0
			// x0 = A.x() + t*B.x()
			// t = (x0 - A.x())/B.x()
			// And the same formula follows for x, y, z, etc.
			// Inverse of direction, equivalent to 1/Bx
			// If ray is parallel to axis, r.direction() = 0
			// and t0, t1 will both be either +inf or -inf if origin is not between x0 and x1
			// a|  b:  |c  -> (a-b) = negative, (c-b) = positive
			// a|  c|  :b  -> (a-b) = negative, (c-b) = negative
			// b:  a|  |c  -> (a-b) = positive, (c-b) = positive
			// If ray is exactly on the boundary, will get 0/0 so NaN, resultin in false.
			// To handle this case, add a very slight padding to box.
			float invD = 1.0f / r.direction()[i];
			float t0 = (_min[i] - r.origin()[i]) * invD;
			float t1 = (_max[i] - r.origin()[i]) * invD;
			// If will only be negative if ray is going in opposite direction of the axis:
			// ex: -1|####|-2  <-------
			// in that case, t1 would happen before t0, so we swap the values
			if (invD < 0.0f) {
				std::swap(t0, t1);
			}
			// Update tmin and tmax to calculate overlap between intervals
			// Two intervals [A0, A1] and [B0, B1] will have overlap if Min(A1, B1) > Max(A0, B0)
			// So, to test all 3 axis, we simply keep track of the max for t_0 and min t_1, and 
			// return false if no overlap
			// Also handles infinity case correctly, will get +inf on tmin or -inf on tmax, so return is false.
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if (tmax <= tmin)
				return false;
		}
		return true;
	}
	vec3 _min;
	vec3 _max;
};

// Creates a bounding box surrounding two boxes
aabb surrounding_box(aabb box0, aabb box1) {
	vec3 small (ffmin(box0._min.x(), box1._min.x()),
		   	    ffmin(box0._min.y(), box1._min.y()),
			    ffmin(box0._min.z(), box1._min.z()));
	vec3 big  (ffmax(box0._max.x(), box1._max.x()),
			   ffmax(box0._max.y(), box1._max.y()),
			   ffmax(box0._max.z(), box1._max.z()));
	return aabb(small, big);
}
#endif