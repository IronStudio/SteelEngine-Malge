#include <iostream>

#include <se/malge/vector.hpp>
#include <se/malge/simd.hpp>



int main(int, char **) {
	se::malge::Vec2i8 vector1 {3};
	std::cout << vector1 << std::endl;

	se::malge::Vec2f32 vector2 {2.f, -3.f};
	std::cout << vector2 << std::endl;

	se::malge::Vec3f32 vector3 {vector2, 1.9f};
	std::cout << vector3 << std::endl;

	se::malge::Vec3f32 vector4 {0.1f};
	std::cout << "vec4 : " << vector4 << std::endl;

	vector3 += vector4;
	std::cout << "vec3 : " << vector3 << std::endl;
	std::cout << vector3 * vector4 << std::endl;
	std::cout << vector3 - vector4 << std::endl;

	std::cout << se::malge::dot(vector3, vector4) << std::endl;
	std::cout << se::malge::cross(vector3, vector4) << std::endl;
	std::cout << se::malge::length2(vector3) << std::endl;
	std::cout << se::malge::length(vector3) << std::endl;


	return 0;
}