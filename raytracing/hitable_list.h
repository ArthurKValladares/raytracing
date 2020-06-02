#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"

class hitable_list : public hitable
{
public:
	hitable_list() {}
	hitable_list(hitable** l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	hitable **list;
	int list_size;
};


bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest = t_max;
	// Iterate trough list, checking for each element if there was a hit
	// If true, then will return true, update the maximum t so that we only look before this element,
	// and return the closes hit found.
	for (int i = 0; i < list_size; ++i) {
		if (list[i]->hit(r, t_min, closest, temp_rec)) 
		{
			hit_anything = true;
			closest = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb& box) const {
	if (list_size < 1) return false;
	aabb temp_box;
	// Create first bounding box
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true) {
		return false;
	} else {
		box = temp_box;
	}
	// iterates trough every element, creating a bounding box for them and surorunding box for the list
	for (int i = 1; i < list_size; ++i) {
		if (list[i]->bounding_box(t0, t1, temp_box)) {
			box = surrounding_box(box, temp_box);
		} else {
			return false;
		}
	}
	return true;
}
#endif // !HITABLE_LIST_H

