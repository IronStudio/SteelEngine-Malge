#include <iostream>

#include <se/malge/vector.hpp>



int main(int, char **) {
	se::malge::Vec2i8 vector1 {3};
	std::cout << vector1 << std::endl;

	se::malge::Vec2f vector2 {2.f, -3.f};
	std::cout << vector2 << std::endl;

	se::malge::Vec3f vector3 {vector2, 1.9f};
	std::cout << vector3 << std::endl;

	return 0;
}