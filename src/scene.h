#pragma once

#include <vector>
#include <memory>
#include "Shape.h"

#include "glm/glm.hpp"


class Scene
{
public:
	std::vector<std::shared_ptr<Shape>> objects;

	void addRoom();


	bool intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
};

