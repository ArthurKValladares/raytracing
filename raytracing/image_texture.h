#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "texture.h"

class image_texture : public texture {
public:
	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
	virtual vec3 value(float u, float v, const vec3& p) const;
	unsigned char* data;
	int nx, ny;
};

vec3 image_texture::value(float u, float v, const vec3& p) const {
	// translate from u [0, 1] to index
	int i = (u)* nx;
	// translate  from v to index.
	// Flipped vertically since our u, v coordinates start at bottom left,
	// While stb starts at top left
	int j = (1 - v) * ny - 0.001;
	// Guarantee indexes are valid
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx - 1) i = nx - 1;
	if (j > ny - 1) j = ny - 1;
	// 3*j*nx represents the vertical position, as j is the vertical index,
	// and each row has 3*nx values (r,g,b for each pixel)
	// 3*i is the horizontal position, as i is the horizontal index
	// We then add +0, +1, +2 to get r, g, b
	float r = int(data[3 * j * nx + 3 * i]) / 255.0;
	float g = int(data[3 * j * nx + 3 * i + 1]) / 255.0;
	float b = int(data[3 * j * nx + 3 * i + 2]) / 255.0;
	return vec3(r, g, b);
}

#endif // !IMAGE__TEXTURE_H

