#pragma once
#include "ray.h"
#include <glm/glm.hpp>
#include <memory>


class Material;

class HitRecord
{
public:
	glm::vec3 p;
	glm::vec3 normal;
	float t;
	std::shared_ptr<Material> mat_ptr;

	glm::vec3 color;

	
};

class Shape
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};


class Sphere : public Shape
{
public:
	glm::vec3 center;
	float radius;
	std::shared_ptr<Material> mat_ptr;

	Sphere(glm::vec3 cen, float r, std::shared_ptr<Material> m);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
};



class Plane : public Shape {
public:
	glm::vec3 point;

	glm::vec3 normal;

	glm::vec3 color;

	std::shared_ptr<Material> mat_ptr;


	Plane(glm::vec3 p, glm::vec3 n, glm::vec3 c, std::shared_ptr<Material> m);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;


};


class Triangle : public Shape
{
public: 
	glm::vec3 v0, v1, v2;
	glm::vec3 normal;
	glm::vec3 color;

	std::shared_ptr<Material> mat_ptr;

	Triangle(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 c, std::shared_ptr<Material> m);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
};


