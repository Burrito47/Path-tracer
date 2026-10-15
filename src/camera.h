#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "ray.h"
#include <string>
#include "scene.h"


class Camera
{
public:

	glm::vec3 origin;



	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;


	int width;
	int height;

	Camera(int img_width, int img_height);


	void render(const std::string& filename, const Scene& scene);

private:

	Ray get_ray(float u, float v) const;

	glm::vec3 get_color(const Ray& r, const Scene& scene, int depth) const;
};

#endif