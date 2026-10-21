#include "camera.h"
#include "material.h"
#include <string>
#include <iostream>

#include <fstream>

Camera::Camera(int img_width, int img_height)
{
	width = img_width;

	height = img_height;


	float aspect_ratio = float(width) / height;

	float viewport_height = 2.0f;

	float viewport_width = aspect_ratio * viewport_height;

	float focal_length = 0.7;

	origin = glm::vec3(0, 0, 0.0f);

	horizontal = glm::vec3(viewport_width, 0, 0);

	vertical = glm::vec3(0, viewport_height, 0);


	lower_left_corner = origin - (horizontal / 2.0f) - (vertical / 2.0f) - glm::vec3(0, 0, focal_length);
}

Ray Camera::get_ray(float u, float v) const
{
	glm::vec3 target = lower_left_corner + (horizontal * u) + (vertical * v);


	return Ray(origin, glm::normalize(target - origin));

}

glm::vec3 Camera::get_color(const Ray& r, const Scene& scene, int depth) const
{
	if (depth <= 0)
	{
		return glm::vec3(0, 0, 0);

	}

	HitRecord rec;


	if (scene.intersect(r, 0.001f, 10000.0f, rec))
	{
		Ray scattered;

		glm::vec3 attenuation;

		glm::vec3 emitted = rec.mat_ptr->emitted();


		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation * get_color(scattered, scene, depth - 1);
		}

		return emitted; // NÄR MAT ABSORBERAR LJUSET

		
	}


	return glm::vec3(0.2f, 0.2f, 0.2f);
}

// bokens lösning till ppm
void Camera::render(const std::string& ppm, const Scene& scene)
{
	std::ofstream file(ppm);

	file << "P3\n" << width << " " << height << "\n255\n";

	std::cout << "Saving pixel count to " << ppm << "..." << std::endl;

	int samples_per_pixel = 500;

	int max_depth = 50;

	for (int j = height; j >= 0; --j)
	{
		glm::vec3 pixel_color(0, 0, 0);
		for (int i = 0; i < width; ++i)
		{

			for (int s = 0; s < samples_per_pixel; ++s)
			{
				float u = float(i + random_float()) / (width - 1);
				float v = float(j + random_float()) / (height - 1);

				Ray r = get_ray(u, v);
				pixel_color += get_color(r, scene, max_depth);

			}

			// delar färgen på antalet strålar för att få medelvärdet
			pixel_color /= float(samples_per_pixel);

			// gamma-korrigering för att ljuset ska bli mjukare
			pixel_color.x = std::sqrt(pixel_color.x);
			pixel_color.y = std::sqrt(pixel_color.y);
			pixel_color.z = std::sqrt(pixel_color.z);

			// clamping
			if (pixel_color.x > 0.999f) pixel_color.x = 0.999f;
			if (pixel_color.y > 0.999f) pixel_color.y = 0.999f;
			if (pixel_color.z > 0.999f) pixel_color.z = 0.999f;

			// gör om färgen från (0.0 - 1.0) till (0 - 255)
			int ir = int(256 * pixel_color.x);
			int ig = int(256 * pixel_color.y);
			int ib = int(256 * pixel_color.z);

			// skriv till filen
			file << ir << " " << ig << " " << ib << "\n";

			std::cout << ir << " " << ig << " " << ib << "\n";

		}
		

	}

	file.close();
	std::cout << "Klar!" << std::endl;
}