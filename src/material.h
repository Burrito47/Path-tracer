#ifndef MATERIAL_H 
#define MATERIAL_h


#include "ray.h"
#include "shape.h"
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <random>


// Kolla Raytracing in one weekend bokens egna material

inline float random_float()
{
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;


	return distribution(generator);
	
}



inline glm::vec3 random_unit_vector()
{
	float z = random_float() * 2.0f - 1.0f;
	float a = random_float() * 2.0f * 3.14;
	float r = sqrt(1.0f - z * z);
	float x = r * cos(a);
	float y = r * sin(a);

	return glm::vec3(x, y, z);
}



class Material
{
public:

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;

	virtual glm::vec3 emitted() const
	{
		return glm::vec3(0, 0, 0);

	}
	
};


class Lambertian : public Material
{
public:
	glm::vec3 albedo;

	Lambertian(const glm::vec3& a) : albedo(a) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
	{

        // Helt random ska ljuset studsa från normal
		glm::vec3 scatter_direction = rec.normal + random_unit_vector();

		scattered = Ray(rec.p, glm::normalize(scatter_direction));
		attenuation = albedo;  // tar ytans färg
		return true;
	}
};

class Metal : public Material {
public:
	glm::vec3 albedo; // reflektivitet

	Metal(const glm::vec3& a) : albedo(a) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {

		glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);

		scattered = Ray(rec.p, glm::normalize(reflected));
		attenuation = albedo;

		// om strålen faktiskt studsar bort från ytan
		return (glm::dot(scattered.direction(), rec.normal) > 0);
	}
};

class Dielectric : public Material {
public:
    float ir; // Brytningsindex

    Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
        // glas absorberar inte färg, det släpper bara igenom ljuset
        attenuation = glm::vec3(1.0, 1.0, 1.0);

        float refraction_ratio;
        glm::vec3 outward_normal;

        // krockar strålen från insidan eller utsidan av glaset?
        if (glm::dot(r_in.direction(), rec.normal) > 0.0f) {
            // inuti glaskulan och är på väg ut
            refraction_ratio = ir;
            outward_normal = -rec.normal; // vänder på normalen
        }
        else {
            // utanför glaskulan och är på väg in
            refraction_ratio = 1.0f / ir;
            outward_normal = rec.normal;
        }

        glm::vec3 unit_direction = glm::normalize(r_in.direction());

        // Snells lag
        float cos_theta = std::fmin(glm::dot(-unit_direction, outward_normal), 1.0f);
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
        glm::vec3 direction;

        // Om ljuset träffar i en för flack vinkel blir det total inre reflektion, 
        // annars använder vi Schlicks approximation för att slumpa mellan spegel och glas.
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float()) {
            direction = glm::reflect(unit_direction, outward_normal);
        }
        else {
            direction = glm::refract(unit_direction, outward_normal, refraction_ratio);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

private:
    // Schlicks
    static float reflectance(float cosine, float ref_idx) {
        float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
    }
};




class DiffuseLight : public Material {
public:
    glm::vec3 emit_color;

    DiffuseLight(const glm::vec3& c) : emit_color(c) {}

    // skickar ljus, reflekterar ej
    bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
        return false;
    }

    // materialets färg blir emitted
    glm::vec3 emitted() const override {
        return emit_color;
    }
};



#endif MATERIAL_H// ! 3