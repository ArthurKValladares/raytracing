#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <limits>
#include <thread>
#include <mutex>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "moving_sphere.h"
#include "bvh_node.h"
#include "texture.h"
#include "const_texture.h"
#include "checker_texture.h"
#include "noise_texture.h"
#include "image_texture.h"
#include "diffuse_light.h"
#include "rect.h"
#include "flip_normals.h"
#include "box.h"
#include "translate.h"
#include "volume.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define max_(x, y) (x > y ? x : y)

// TODO:
// BOOK 2
// LATER< MULTITHREADING

void final(hitable** scene, camera** cam, float aspect) {
	int nb = 20;
	hitable** list = new hitable * [30];
	hitable** boxlist = new hitable * [10000];
	hitable** boxlist2 = new hitable * [10000];
	material* white = new lambertian(new const_texture(vec3(0.73, 0.73, 0.73)));
	material* ground = new lambertian(new const_texture(vec3(0.48, 0.83, 0.53)));
	int b = 0;
	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < nb; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = 100.0*(random_double()+ 0.01);
			auto z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	material* light = new diffuse_light(new const_texture(vec3(4, 4, 4)));
	list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
	vec3 center(400, 400, 200);
	list[l++] = new moving_sphere(
		center, center + vec3(30, 0, 0), 0, 1, 50,
		new lambertian(new const_texture(vec3(0.7, 0.3, 0.1)))
	);
	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(vec3(1.0, 1.0, 1.0), 1.5));
	list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));
	hitable* boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(vec3(1.0, 1.0, 1.0), 1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2, new const_texture(vec3(0.2, 0.4, 0.9)));
	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(vec3(1.0, 1.0, 1.0), 1.5));
	list[l++] = new constant_medium(boundary, .0001, new const_texture(vec3(1.0, 1.0, 1.0)));
	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material* emat = new lambertian(new image_texture(tex_data, nx, ny));
	list[l++] = new sphere(vec3(400, 200, 400), 100, emat);
	texture* pertext = new noise_texture(0.1);
	list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));

	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxlist2[j] = new sphere(vec3(165*random_double(), 165 * random_double(), 165 * random_double()), 10, white);
	}

	list[l++] = new translate(
		new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));

	vec3 look_from(278, 278, -800);
	vec3 look_at(278, 278, 0);
	vec3 vup(0, 1, 0);
	float dist_to_focus = 10.0;
	float aperture = 0;
	float vfov = 40.0;
	float time0 = 0.0;
	float time1 = 1.0;
	*cam = new camera(look_from, look_at, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);
	*scene = new hitable_list(list, l);
}

void cornell_smoke(hitable** scene, camera** cam, float aspect) {
	hitable** list = new hitable * [8];
	int i = 0;
	material* red = new lambertian(new const_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new const_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new const_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new const_texture(vec3(4.0, 4.0, 4.0)));

	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	hitable* b1 = new translate(
		new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
		vec3(130, 0, 65));
	hitable* b2 = new translate(
		new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(265, 0, 295));

	list[i++] = new constant_medium(
		b1, 0.01, new const_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(
		b2, 0.01, new const_texture(vec3(0.0, 0.0, 0.0)));

	vec3 look_from(278, 278, -800);
	vec3 look_at(278, 278, 0);
	vec3 vup(0, 1, 0);
	float dist_to_focus = 10.0;
	float aperture = 0;
	float vfov = 40.0;
	float time0 = 0.0;
	float time1 = 1.0;
	*cam = new camera(look_from, look_at, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);
	*scene = new hitable_list(list, i);
}

void cornell_box(hitable **scene, camera **cam, float aspect) {
	hitable** list = new hitable * [8];
	int i = 0;
	material* red = new lambertian(new const_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new const_texture(vec3(1.0,1.0,1.0)));
	material* green = new lambertian(new const_texture(vec3(0.12, 0.45, 0.15)));
	material*  light = new diffuse_light(new const_texture(vec3(4.0, 4.0, 4.0)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	// Create box
	list[i++] = new translate(
		new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
		vec3(130, 0, 65)
	);
	list[i++] = new translate(
		new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(265, 0, 295)
	);
	vec3 look_from(278, 278, -800);
	vec3 look_at(278, 278, 0);
	vec3 vup(0, 1, 0);
	float dist_to_focus = 10.0;
	float aperture = 0;
	float vfov = 40.0;
	float time0 = 0.0;
	float time1 = 1.0;
	*cam = new camera(look_from, look_at, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);
	*scene = new hitable_list(list, i);
}

vec3 get_color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	// We use 0.001 as the lower limit here to fix the shadow-acne problem
	// This happens since due to floating point rounding errors, out intersection point
	// instead of being exactly on top of the surface, can be computed to be slightly below the surface.
	// When that happens, the resulting ray then can bounce in the direction of the surface itself,
	// and create a case of self intersection.
	// Then to fix this, we simply ignore results too close to 0.
	if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		// We put a cap on number of reflections, to keep from bouncing infinitely
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			// If ray was scattered, we then attenuate it according to the material's property,
			// and get the color of the reflected ray, incrementing the depth of reflections.
			//return attenuation;
			return attenuation * get_color(scattered, world, depth + 1);
		} 
		else
		{
			// If ray did not reflect or depth is too great, just return emitted value.
			return emitted;
		}
	}
	else
	{
		/*
		// Normalized direction vector for ray, range [-1, 1]
		vec3 unit_direction = unit_vector(r.direction());
		// map range to [0, 1]
		float t = 0.5 * (unit_direction.y() + 1.0);
		// lerp of blue (second member, when y vector = 1) and white (first member, when y vector = 0)
		return (1.0-t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
		*/
		return vec3(0.0, 0.0, 0.0);
	}
}

// EAch thread will write the first available row to them, then go write to the next free row
void raytrace_and_write(hitable *world, camera *cam, int *buffer, 
	int size_x, int size_y, int samples,
	int* next_tile_x, int* next_tile_y, int tile_width, int tile_height,
	std::mutex* tile_mutex) {
	while (true) {
		tile_mutex->lock();
		int write_x = *next_tile_x;
		int write_y = *next_tile_y;
		*next_tile_x += tile_width;
		if (*next_tile_x >= size_x) {
			*next_tile_x = 0;
			*next_tile_y += tile_height;
		}
		tile_mutex->unlock();
		if (write_y >= size_y) {
			return;
		}
		tile_width = tile_width - max_(0, (write_x + tile_width) - size_x);
		tile_height = tile_height - max_(0, (write_y + tile_height) - size_y);
		for (int pst_y = write_y; pst_y < write_y + tile_height; ++pst_y)
		{
			for (int pst_x = write_x; pst_x < write_x + tile_width; ++pst_x)
			{
				vec3 color(0, 0, 0);
				// MSAA Anti-alliasing, taking size_s samples around the pixel, 
				// and averaging their results.
				for (int s = 0; s < samples; ++s) {
					// percentage displacement of (x + random shift in range [0, 1))
					float u = float(pst_x + random_double()) / float(size_x);
					// percentage displacement of (y + random shift in range [0, 1))
					float v = float(pst_y + random_double()) / float(size_y);
					// The range [0, 1) is important because no matter what value we get,
					// it will still be inside the same pixel we are processing, as we
					// start our ray on the lower left corner of the pixel.

					// Ray starting at camera, pointin at where we take our sample
					ray r = cam->get_ray(u, v);
					// p unused so far
					vec3 p = r.point_at_parameter(2.0);
					// Increment overall color of the pixel by color we get from this sample.
					color += get_color(r, world, 0);
				}
				// Normalizing color so we get values in range [0,1]
				color /= float(samples);
				// CLOSELY RELATE TO THIS

				// Gamma correcting our color with 'gamma 2'
				// i.e, raising the color to the power 1/gamma, or 1/2
				color = vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
				int i_r = int(255.99 * color[0]);
				int i_g = int(255.99 * color[1]);
				int i_b = int(255.99 * color[2]);
				//out_file << i_r << " " << i_g << " " << i_b << "\n";
				int idx = ((size_y - 1) - pst_y) * size_x * 3 + pst_x * 3;
				buffer[idx] = i_r;
				buffer[idx + 1] = i_g;
				buffer[idx + 2] = i_b;
			}
		}
	}
}

int main()
{
	clock_t tStart = clock();

	// Add way to chose seed later
	std::ofstream out_file("output.ppm");
	int size_x = 300;
	int size_y = 300;
	int size_s = 100;
	out_file << "P3\n" << size_x << " " << size_y << "\n255\n";
	hitable* world;
	camera *cam;
	float aspect = float(size_x) / float(size_y);
	final(&world, &cam, aspect);
	/*
	// Vector position for Camera.
	//vec3 look_from(13, 2, 3);
	vec3 look_from(278, 278, -800);
	// Vector position for where camera is pointing at
	//vec3 look_at(0, 0, 0);
	vec3 look_at(278, 278, 0);
	// Vector that defines up direction for camera
	// We project this vector on the plane orthogonal to the view direction
	// to get a vector poitting "up"  from where camera is positioned.
	vec3 vup(0, 1, 0);
	float fov = 40.0;
	//float dist_to_focus = (look_from - look_at).length();
	float dist_to_focus = 10;
	// Aperture is the diametrer of the camera hole. 
	float aperture = 0.0;
	// Time for camera goes from 0 to 1.
	float time0 = 0.0;
	float time1 = 1.0;
	camera cam(look_from, look_at, vup, fov, float(size_x)/float(size_y), aperture, dist_to_focus, time0, time1);

	// CAN THIS LOOP ALSO BE MULTITHREADED?
	//hitable* world = random_scene(-2, 2, -2, 2, time0, time1);
	//hitable* world = random_scene_blur(0, 0, 0, 0, time0, time1);
	//hitable* world = two_spheres();
	//hitable* world = two_perlin_spheres();
	//hitable* world = earth();
	//hitable* world = simple_light();
	//hitable* world = cornell_smoke();
	//hitable* world = final();
	*/

	int num_threads = std::thread::hardware_concurrency();
	if (num_threads == 0) num_threads = 1;
	std::thread* threads = new std::thread[num_threads];
	int* buffer = new int[size_x * size_y * 3];
	int next_x = 0;
	int next_y = 0;
	int tile_width = 32;
	int tile_height = 32;
	std::mutex tile_info;
	for (size_t i = 0; i < num_threads; ++i) {
		threads[i] = std::thread(raytrace_and_write, world, cam, buffer, size_x, size_y, size_s, &next_x, &next_y, tile_width, tile_height, &tile_info);
	}
	for (size_t i = 0; i < num_threads; ++i) {
		threads[i].join();
	}
	// Write from buffer to file
	for (size_t i = 0; i < size_x * size_y * 3; i += 3) {
		out_file << buffer[i] << " " << buffer[i + 1] << " " << buffer[i + 2] << "\n";
	}
	// Write result to file
	//stbi_write_bmp("output.bmp", size_x, size_y, 3, buffer);
	delete[] buffer;
	/*
	for (int pst_y = size_y - 1; pst_y >= 0; --pst_y) 
	{
		for (int pst_x = 0; pst_x < size_x; ++pst_x)
		{
			vec3 color(0, 0, 0);
			// MSAA Anti-alliasing, taking size_s samples around the pixel, 
			// and averaging their results.
			for (int s = 0; s < size_s; ++s) {
				// percentage displacement of (x + random shift in range [0, 1))
				float u = float(pst_x + random_double()) / float(size_x);
				// percentage displacement of (y + random shift in range [0, 1))
				float v = float(pst_y + random_double()) / float(size_y);
				// The range [0, 1) is important because no matter what value we get,
				// it will still be inside the same pixel we are processing, as we
				// start our ray on the lower left corner of the pixel.

				// Ray starting at camera, pointin at where we take our sample
				ray r = cam->get_ray(u, v);
				// p unused so far
				vec3 p = r.point_at_parameter(2.0);
				// Increment overall color of the pixel by color we get from this sample.
				color += get_color(r, world, 0);
			}
			// Normalizing color so we get values in range [0,1]
			color /= float(size_s);
			// CLOSELY RELATE TO THIS
			
			// Gamma correcting our color with 'gamma 2'
			// i.e, raising the color to the power 1/gamma, or 1/2
			color = vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
			int i_r = int(255.99 * color[0]);
			int i_g = int(255.99 * color[1]);
			int i_b = int(255.99 * color[2]);
			out_file << i_r << " " << i_g << " " << i_b << "\n";
		}
	}
	*/
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	return 0;
}