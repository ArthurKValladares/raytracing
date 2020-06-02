#ifndef CONST_TEXTURE_H
#define CONST_TEXTURE_H

class const_texture : public texture {
public:
	const_texture() {}
	const_texture(vec3 c) : color(c) {}
	virtual vec3 value(float u, float v, const vec3& p) const { return color; }
	vec3 color;
};

#endif // !CONST_TEXTURE_H

