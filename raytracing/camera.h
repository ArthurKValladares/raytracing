#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

// TODO: REDUNDANDE DEFINE CAMERA/SPHERE, SIMPLIFY LATER
#define PI 3.1415926535897

// Generates a random point in a unit disk (z = 0)
vec3 random_in_unit_disk()
{
	vec3 p;
	do
	{
		// Generate a random point in a square with x e [-1, 1], y e [-1, 1]
		p = 2.0 * vec3(random_double(), random_double(), 0) - vec3(1.0, 1.0, 0);
	} while (dot(p, p) >= 1.0); // Continue until the length of the vector from the origin is < 1
	return p;
}

class camera
{
public:
	camera(vec3 look_from, vec3 look_at, vec3 v_up, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) 
	{
		// Start time for the camera
		time0 = t0;
		// End time for the camera
		time1 = t1;
		lens_radius = aperture / 2;
		// Corverting out vertical fov from degrees to radians
		float theta = vfov * PI / 180;
		// Simple trigonometry, by drawing the vertical angle of our view camera and a plane
		// perpendicular to its look direction vector at distance 1.
		float half_height = tan(theta / 2);
		// aspect is size_horizontal/size_vertical.
		float half_width = aspect * half_height;
		// Origin for the camera is where camera is looking from
		origin = look_from;
		// u, v, w make up an orthonoral basis for our camera orientation.
		// w points behind the camera
		w = unit_vector(look_from - look_at);
		// u points "horizontally" from the camera
		// We get it from the cross product of w and v_up, 
		// since that's the only direction orthogonal to both up and behind.
		// NOTE: Technically v_up is not the real up vector for our camera, but since 
		// v is just a projection of v_up, they are equivalent in this case, as
		// w, v_up, and v are in the same plane.
		u = unit_vector(cross(v_up, w));
		// the v points up from the camera.
		// It's simple to get v since we already have 2 of the 3 orthonormal vectors
		// for our camera orientation.
		v = cross(w, u);
		// As we assume focus plane is at distance 1 in camera setup, we must then scale our vectors by
		// the actual focus distance.
		// To get the lower left corner of our camera view, we subtract its origin by the orthogonal vectors 
		// of its orientation, scaled by their distance (focus_dist) and the size of the corresponding direction.
		// we subtract because our camera faces (-w) and the lower left corner of the image is in the opposite
		// direction of v (down intead of up) and u (left intead of right).
		lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
		horizontal = 2*half_width*focus_dist*u;
		vertical = 2*half_height*focus_dist*v;
	}

	// s and t represent the percentual displacement in horizontal and vertical directions
	ray get_ray(float s, float t) 
	{
		// Lens radius is half the aperture of the camera, and it controls the "depth of fiels" effect
		// Here we get a displacement for the camera ray, instead of starting from exactly the origin, 
		// we start from a point inside the aperture specified (a disk with radius lens_radius)
		vec3 rd = lens_radius*random_in_unit_disk();
		// We then trasnfer that point to our camera's coordinate system.
		vec3 offset = u*rd.x() + v*rd.y();
		// Get the point in time in given interval
		float time = time0 + random_double() * (time1 - time0);
		// return a ray starting from that new position, pointing at the point specified by s and t, at the time given
		return ray(origin + offset, (lower_left_corner + s*horizontal + t*vertical) - (origin + offset), time); 
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius;
	float time0, time1;
};

#endif 