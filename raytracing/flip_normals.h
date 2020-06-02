#ifndef FLIP_NORMALS_H
#define FLIP_NORMALS_H

class flip_normals : public hitable {
public:
	flip_normals(hitable* p): ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		return false;
	}
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		return ptr->bounding_box(t0, t1, box);
	}
	hitable* ptr;
};

#endif