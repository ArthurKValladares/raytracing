#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <algorithm>
#include "hitable.h"
#include "aabb.h"

// tree-like structure to hold bounding box hierachy
// Bounding-volume-hierarchy
class bvh_node : public hitable {
	friend bool box_x_compare(hitable* ah, hitable* bh);
	friend bool box_y_compare(hitable* ah, hitable* bh);
	friend bool box_z_compare(hitable* ah, hitable* bh);
public:
	bvh_node() {}
	// Creates bvh from a list of pointers to hitables
	bvh_node(hitable** l, int n, float time0, float time1);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& b) const;
	hitable* left;
	hitable* right;
	aabb box;
};

// Compare functions
int box_x_compare(const void* a, const void* b) {
	aabb box_left, box_right;
	// cast void pointers back to (hitable **), then dereference it to (hitable *)
	hitable* ah = *(hitable **)a;
	hitable* bh = *(hitable **)b;
	// throw error if hitables don't have bounding boxes
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	// Sort according to position in axis
	if (box_left.min().x() - box_right.min().x() < 0.0)
		return -1;
	else
		return 1;
}

int box_y_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	hitable* ah = *(hitable **)a;
	hitable* bh = *(hitable **)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().y() - box_right.min().y() < 0.0)
		return -1;
	else
		return 1;
}
int box_z_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	hitable* ah = *(hitable **)a;
	hitable* bh = *(hitable **)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().z() - box_right.min().z() < 0.0)
		return -1;
	else
		return 1;
}

bvh_node::bvh_node(hitable** l, int n, float time0, float time1) {
	// Randomly choose an axis
	int axis = int(3 * random_double());
	// Sort the primitives along that axis
	if (axis == 0) {
		qsort(l, n, sizeof(hitable*), box_x_compare);
	} else if (axis == 1) {
		qsort(l, n, sizeof(hitable*), box_y_compare);
	} else {
		qsort(l, n, sizeof(hitable*), box_z_compare);
	}
	// Set left and right nodes
	if (n == 1) {
		left = right = l[0];
	} else if (n == 2) {
		left = l[0];
		right = l[1];
	} else {
		// Divide the list in 2 halves.
		left = new bvh_node(l, n / 2, time0, time1);
		right = new bvh_node(l + n / 2, n - n / 2, time0, time1);
	}
	// If one of the nodes doesn't have a box, throw error
	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
		std::cerr << "No bounding box in bvh_node constructor\n";
	}
	box = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	// Check if box at root hit
	if (box.hit(r, t_min, t_max)) {
		// If yes, check children
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);
		// Both hit, save earliest hit, just one hit, return that hit
		if (hit_left && hit_right) {
			if (left_rec.t < right_rec.t) {
				rec = left_rec;
			}
			else {
				rec = right_rec;
			}
			return true;
		} else if (hit_left) {
			rec = left_rec;
			return true;
		} else if (hit_right) {
			rec = right_rec;
			return true;
		}
	}
	return false;
}

// Box built at construction, just use root box
bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
	b = box;
	return true;
}
#endif // !BVH_NODE_H

