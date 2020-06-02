#ifndef VEC3_H
#define VEC3_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "random.h"

class vec3
{
public:
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	// Const function, used when not changing vector value
	inline float operator[](int i) const { return e[i]; }
	// Returns reference to vector element, used when changing value
	inline float& operator[](int i) { return e[i]; }

	inline vec3& operator+=(const vec3& v);
	inline vec3& operator-=(const vec3& v);
	inline vec3& operator*=(const vec3& v);
	inline vec3& operator/=(const vec3& v);
	inline vec3& operator*=(const float c);
	inline vec3& operator/=(const float c);

	inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
	inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
	inline void make_unit_vector();

	float e[3];
};

inline std::istream& operator>>(std::istream& is, vec3& v)
{
	is >> v.e[0] >> v.e[1] >> v.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, vec3& v)
{
	os << v.e[0] << " " << v.e[1] << " " << v.e[2];
	return os;
}

inline vec3 operator+(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0]+v2.e[0],
		        v1.e[1]+v2.e[1],
		        v1.e[2]+v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0]-v2.e[0],
		        v1.e[1]-v2.e[1],
		        v1.e[2]-v2.e[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0]*v2.e[0],
		        v1.e[1]*v2.e[1],
		        v1.e[2]*v2.e[2]);
}

inline vec3 operator*(const float c, const vec3& v)
{
	return vec3(c*v.e[0],
		        c*v.e[1],
		        c*v.e[2]);
}

inline vec3 operator*(const vec3& v, const float c)
{
	return vec3(c*v.e[0],
		        c*v.e[1],
		        c*v.e[2]);
}

inline vec3 operator/(const vec3& v, const float c)
{
	return vec3(v.e[0]/c,
		        v.e[1]/c,
		        v.e[2]/c);
}

inline vec3 operator/(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0]/v2.e[0],
		        v1.e[1]/v2.e[1],
		        v1.e[2]/v2.e[2]);
}

inline vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3((v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
		       -(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0]),
		        (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}

inline float dot(const vec3& v1, const vec3& v2)
{
	return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline vec3 unit_vector(const vec3& v)
{
	return v / v.length();
}

// Should this be inline?
inline vec3 reflect(const vec3& v, const vec3& n)
{
	// Formula for reflection = v - 2 * (projection of v on n)
	//             reflection = v - 2 * dot(v, n)/dot(n, n)*n
	// since n unit length,   = v - 2 * dot(v, n) * n
	return v - 2 * dot(v, n) * n;
}


// Comes from Snell's law, where n_1*sin(thetha_1) = n_2*sin(thetha_2)
// where, n_1, thetha_1, are refractive index and angle on incident material
// and,   n_2, thetha_2, are refractive index and angle on refracted material
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	// We make v a unit vector to simplify calculations.
	// since n is already an unit vector, cos(theta) = dot(v, n),
	// and then since sin(thetha) = sqrt(1-cos(thetha)^2) = sqrt(1-dot(v, n)^2)
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	// The discriminant represent the situations in which Snell's law has no real solutions
	// which happens when the refractive index for the incident material (n_1) is higher than
	// the refracted material (n_1)
	// We see this mathematically in our Snell's equation of the form:
	// n_1*sqrt(1-dot(v_1*n)) = n_2*sqrt(1-dot(v_2,  n)^2)
	// Which we can re-arrange in the equivalent form: (separating unkowns and known variables)
	// (n_1/n_2)^2*(1-dot(v_1, n)^2) = 1 - dot(v_2, n)^2
	// Since dot(v_2, n)^2 is strictly positive, then:
	// (n_1/n_2)^2*(1-dot(v_1, n)^2)
	// must be smaller than 1.
	float discriminat = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminat > 0.0)
	{
		// To get the equation of the refracted vector, we first have to decompose it
		// into its component parallel to the surface and normal to the surface.
		// v_2 = v_2_t + v_2_p
		// Now, since we have both out v vectors with unit length, we can see that:
		// sin(thetha_1) = ||v_1_p||  and  sin(thetha_2) = ||v_2_p||
		// Subbing the in Snells's law an re-arranging:
		// ||v_2_p|| = n1/n2*||v_1_p||
		// Since both vector are parallel and point in the same direction:
		// v_2_p = n1/n2*(v_1 - (projection of v1 on n))
		// v_2_p = n1/n2*(v_1 - dot(v_1, n)*n)

		// For the perpendicular vector, we first use Pythagoras:
		// ||v_2||^2 =  ||v_2_t||^2 + ||v_2_p||^2
		// since v_2 is unit length,
		// 1 = ||v_2_t||^2 + ||v_2_p||^2
		// ||v_2_t||^2 = 1 - ||v_2_p||^2
		// ||v_2_t|| = sqrt(1-||v_2_p||^2)
		// Since this vector is parellel to the normal, and point in the opposite direction,
		// we can just scale it's magnitude by (-n) to get:
		// v_2_t = sqrt(1-||v_2_p||^2)*(-n)
		// As we perviously established:
		// ||v_2_p|| = n1/n2*||v_1_p|| = n1/n2*sin(thetha_1)
		// ||v_2_p|| = n1/n2*(sqrt(1 - cos*(thetha_1)^2))
		// ||v_2_p|| = (n1/n2)*sqrt(1 - dot(n, v)^2)
		// ||v_2_p||^2 = (n1/n2)^2*(1 - dot(n, v)^2)

		// Adding the components together we get:
		// v_2 = n1/n2*(v_1 - dot(v_1, n)*n) - sqrt(1-||v_p_2||^2)*(n)
		// v_2 = n1/n2*(v_1 - dot(v_1, n)*n) - sqrt(1-(n1/n2)^2*(1 - dot(v_1, n)^2))*(n)
		// since discriminant = 1-(n1/n2)^2*(1 - dot(v_1, n)^2)
		// v_2 = n1/n2*(v_1 - dot(v_1, n)*n) - sqrt(discriminant)*(n)
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminat);
		return true;
	}
	return false;
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do
	{
		// The vector with a random value in all 3 directons picks a point
		// in the random unit cube where all directions range from [0, 1]
		// Then, we map that point to the range [-1, 1] by doing (2*p - 1)
		// and get a point in the cube where all direction range from [-1, 1]
		// We then use a rejection algorithm to only accept point in that cube that 
		// are also inside the unit sphere, by only acception point with (length^2 < 1)
		p = 2.0* vec3( random_double(), random_double(), random_double()) - vec3(1.0, 1.0, 1.0);
	} while (p.squared_length() >= 1.0);
	return p;
}



inline vec3& vec3::operator+=(const vec3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& v) 
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3& v) 
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3& v)
{
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const float c)
{
	e[0] *= c;
	e[1] *= c;
	e[2] *= c;
	return *this;
}

inline vec3& vec3::operator/=(const float c)
{
	float inv = 1.0 / c;
	e[0] *= inv;
	e[1] *= inv;
	e[2] *= inv;
	return *this;
}

inline void vec3::make_unit_vector()
{
	float norm = 1 / this->length();
	e[0] *= norm;
	e[1] *= norm;
	e[2] *= norm;
}

vec3 random_on_unit_sphere() {
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_cosine_direction() {
	float r1 = random_double();
	float r2 = random_double();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}
#endif // !VEC3_H

