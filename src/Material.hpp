#include <glm/glm.hpp>
#include <random>


inline float random_float()
{
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;

	return distribution(generator);
}


inline glm::vec3 random_unit_vector()
{
	
	float z = random_float() * 2.0f - 1.0f;
	float a = random_float() * 2.0f * 3.14f;
	float r = sqrt(1.0f - z * z);
	float x = r * cos(a);
	float y = r * sin(a);
	return glm::vec3(x, y, z);
}


class Material {
public:
	virtual bool scatter(const Ray& r_in)
};