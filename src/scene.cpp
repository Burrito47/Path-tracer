#include "scene.h"





void Scene::addRoom()
{


	objects.push_back(std::make_shared<Sphere>(glm::vec3(0.4, -0.6, -0.3), 0.3));



}



bool Scene::intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;

	float cosest_so_far = t_max;


	for (const auto& object : objects)
	{
		if (object->hit(r, t_min, cosest_so_far, temp_rec))
		{
			hit_anything = true;
			cosest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

