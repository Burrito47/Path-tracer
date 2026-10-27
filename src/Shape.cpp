#include "Shape.h"
#include <memory>



Sphere::Sphere(glm::vec3 cen, float r,  std::shared_ptr<Material> m)
{
	center = cen;

	radius = r;


	mat_ptr = m;



}

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	glm::vec3 oc = r.origin() - center;
	float a = glm::dot(r.direction(), r.direction());
	float half_b = glm::dot(oc, r.direction());
	float c = glm::dot(oc, oc) - radius * radius;

	float discriminant = half_b * half_b - a * c;
	// åtminstone en punk om positiv diskriminant

	if (discriminant > 0)
	{
		float root = sqrt(discriminant);

		// kolla framsidan dvs första trfpunkt
		float temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;

		}
		temp = (-half_b + root) / a;

		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;

			return true;
		}
	}
	return false;
}

Plane::Plane(glm::vec3 p, glm::vec3 n, glm::vec3 c, std::shared_ptr<Material> m) {
	point = p;
	normal = n;
	color = c;
	mat_ptr = m;
}
bool Plane::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float denominator = glm::dot(r.direction(), normal);


	if (std::abs(denominator) > 0.0001f)
	{
		glm::vec3 vector_to_plane = point - r.origin();

		float t = glm::dot(vector_to_plane, normal) / denominator;

		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.color = color;
			rec.normal = normal;
			rec.p = r.at(t);
			rec.mat_ptr = mat_ptr;

			return true;
		}
	}

	return false;
}


Triangle::Triangle(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 c, std::shared_ptr<Material> m)
{

	v0 = vert0;
	v1 = vert1;
	v2 = vert2;
	color = c;
	mat_ptr = m;

	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;

	normal = glm::normalize(glm::cross(edge1, edge2));


}

bool Triangle::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	// orginal 0.0000001f men vi vill förhindra "shadow acne" därmed en mindre värde
	const float EPSILON = 0.0000001f;


	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;

	glm::vec3 h = glm::cross(r.direction(), edge2);

	float a = glm::dot(edge1, h);

	if (a > -EPSILON && a < EPSILON)
	{
		return false;
	}


	float f = 1.0f / a;

	glm::vec3 s = r.origin() - v0;

	float u = f * glm::dot(s, h);

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}


	glm::vec3 q = glm::cross(s, edge1);
	float v = f * glm::dot(r.direction(), q);

	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}


	float t = f * glm::dot(edge2, q);

	if (t > t_min && t < t_max)
	{
		rec.t = t;
		rec.p = r.at(t);

		rec.color = color;

		rec.mat_ptr = mat_ptr;
		rec.normal = normal;

		return true;
	}
	return false;
}

// update