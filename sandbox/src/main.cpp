#include <chrono>
#include <iostream>
#include <vector>

#include <se/malge/vector.hpp>
#include <se/malge/matrix.hpp>
#include <se/malge/simd.hpp>


struct Chrono {
	Chrono(const std::string &argName) :
		name {argName},
		start {std::chrono::steady_clock::now()}
	{

	}
	~Chrono() {
		auto end {std::chrono::steady_clock::now()};
		std::cout << "Chrono '" << name << "' : " << std::chrono::duration_cast<std::chrono::duration<float>> (end - start).count() << " [s]" << std::endl;
	}

	std::string name;
	std::chrono::steady_clock::time_point start;
};


int main(int, char **) {
	/*se::malge::Vec2i8 vector1 {3};
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
	std::cout << se::malge::length(vector3) << std::endl;*/


	se::malge::Mat3f matrix1 {1.f};
	std::cout << matrix1 << std::endl;
	std::cout << "---------" << std::endl;

	se::malge::Mat3f matrix2 {};
	std::cout << matrix2 << std::endl;
	std::cout << "---------" << std::endl;

	se::malge::Mat3f matrix3 {
		1.f, 2.f, 3.f,
		4.f, 5.f, 6.f,
		7.f, 8.f
	};
	std::cout << matrix3 << std::endl;
	std::cout << "---------" << std::endl;

	matrix1 += matrix3;
	std::cout << matrix1 << std::endl;
	std::cout << "---------" << std::endl;

	matrix1 -= matrix3;
	std::cout << matrix1 << std::endl;
	std::cout << "---------" << std::endl;

	matrix3 *= se::malge::Mat3f (1.f);
	std::cout << matrix3 << std::endl;
	std::cout << "---------" << std::endl;




	/*constexpr size_t MATRIX_COUNT {1000000};
	float FILL_VALUE {109823.38412f};
	std::vector<float> toCopy {};
	toCopy.resize(MATRIX_COUNT * 16 + 15);
	float *alignedToCopy {toCopy.data() + (size_t)(toCopy.data()) % 16};

	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16);
		memset(data.data(), 0, sizeof(float) * MATRIX_COUNT * 16);

		Chrono chrono {"Naive filling"};
		for (size_t i {0}; i < MATRIX_COUNT * 16; ++i) {
			data[i] = FILL_VALUE;
		}
	}


	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16 + 15);
		memset(data.data(), 0, sizeof(float) * MATRIX_COUNT * 16 + 15);
		float *startAddress {data.data() + (size_t)(data.data()) % 16};

		Chrono chrono {"SIMD filling"};

		for (size_t i {0}; i < MATRIX_COUNT * 16; i += 16) {
			__m128 reg {_mm_load_ps1(&FILL_VALUE)};
			_mm_store_ps(startAddress + i, reg);
			_mm_store_ps(startAddress + i + 4, reg);
			_mm_store_ps(startAddress + i + 8, reg);
			_mm_store_ps(startAddress + i + 12, reg);
		}
	}



	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16);
		Chrono chrono {"memset erasing"};

		for (size_t i {0}; i < MATRIX_COUNT * 16; i += 16)
			memset(data.data() + i, 0, sizeof(float) * 16);
	}


	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16 + 15);
		float *startAddress {data.data() + (size_t)(data.data()) % 16};
		Chrono chrono {"SIMD erasing (setzero)"};

		for (size_t i {0}; i < MATRIX_COUNT * 16; i += 16) {
			__m128 reg {_mm_setzero_ps()};
			_mm_store_ps(startAddress + i, reg);
			_mm_store_ps(startAddress + i + 4, reg);
			_mm_store_ps(startAddress + i + 8, reg);
			_mm_store_ps(startAddress + i + 12, reg);
		}
	}

	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16 + 15);
		float *startAddress {data.data() + (size_t)(data.data()) % 16};
		Chrono chrono {"SIMD erasing (xor)"};

		for (size_t i {0}; i < MATRIX_COUNT * 16; i += 16) {
			__m128 reg {};
			reg = _mm_xor_ps(reg, reg);
			_mm_store_ps(startAddress + i, reg);
			_mm_store_ps(startAddress + i + 4, reg);
			_mm_store_ps(startAddress + i + 8, reg);
			_mm_store_ps(startAddress + i + 12, reg);
		}
	}




	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16);
		Chrono chrono {"memcpy copy"};

		for (size_t i {0}; i < MATRIX_COUNT * 16; i += 16)
			memcpy(data.data() + i, toCopy.data() + i, sizeof(float) * 16);
	}


	{
		std::vector<float> data {};
		data.resize(MATRIX_COUNT * 16 + 15);
		float *startAddress {data.data() + (size_t)(data.data()) % 16};
		Chrono chrono {"SIMD copy"};

		for (size_t i {0}; i < MATRIX_COUNT * 16; i += 16) {
			auto reg1 {_mm_load_ps(alignedToCopy + i)};
			auto reg2 {_mm_load_ps(alignedToCopy + i + 4)};
			auto reg3 {_mm_load_ps(alignedToCopy + i + 8)};
			auto reg4 {_mm_load_ps(alignedToCopy + i + 12)};
			_mm_store_ps(startAddress + i, reg1);
			_mm_store_ps(startAddress + i + 4, reg2);
			_mm_store_ps(startAddress + i + 8, reg3);
			_mm_store_ps(startAddress + i + 12, reg4);
		}
	}*/


	return 0;
}