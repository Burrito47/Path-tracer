#include "scene.h"
#include "material.h"
#include <thread>



void Scene::addRoom()
{


    // Golv (grått)
    objects.push_back(std::make_shared<Plane>(
        glm::vec3(0, -1, 0), glm::vec3(0, 1, 0), glm::vec3(0.5, 0.5, 0.5),
        std::make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5))
    ));

    // Tak (grön)
    objects.push_back(std::make_shared<Plane>(
        glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0.1, 0.8, 0.1),
        std::make_shared<Lambertian>(glm::vec3(0.1, 0.8, 0.1))
    ));

    // Bakvägg (grå)
    objects.push_back(std::make_shared<Plane>(
        glm::vec3(0, 0, -1), glm::vec3(0, 0, 1), glm::vec3(0.6, 0.6, 0.6),
        std::make_shared<Lambertian>(glm::vec3(0.6, 0.6, 0.6))
    ));

    // Vänster vägg (röd)
    objects.push_back(std::make_shared<Plane>(
        glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0.8, 0.1, 0.1),
        std::make_shared<Lambertian>(glm::vec3(0.8, 0.1, 0.1))
    ));

    // Höger vägg (blå)
    objects.push_back(std::make_shared<Plane>(
        glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0.1, 0.1, 0.8),
        std::make_shared<Lambertian>(glm::vec3(0.1, 0.1, 0.8))
    ));

    // lampa i taket
    objects.push_back(std::make_shared<Sphere>(
        glm::vec3(0, 1.0, -0.5), 0.15,
        std::make_shared<DiffuseLight>(glm::vec3(100.0, 100.0, 100.0))
    ));

    // vänster sphere (glaskula)
    objects.push_back(std::make_shared<Sphere>(
        glm::vec3(0.4, -0.6, -0.3), 0.3,
        std::make_shared<Dielectric>(1.5f)
    ));

    // höger sphere (metal)
    // objects.push_back(std::make_shared<Sphere>(
    //     glm::vec3(0.4, -0.6, -0.5), 0.3, 
    //     std::make_shared<Metal>(glm::vec3(0.9, 0.9, 0.9)) 
    // ));

    // Tetraeder (metall) 
    // definierar de fyra hörnen (v0, v1, v2 på golvet, v3 är toppen)
    glm::vec3 v0(-0.725f, -0.80f, -0.51f); // Vänster bakre hörn 
    glm::vec3 v1(-0.275f, -0.80f, -0.51f); // Höger bakre hörn 
    glm::vec3 v2(-0.50f, -0.80f, -0.18f); // Främre hörnet (pekar mot kameran)
    glm::vec3 v3(-0.50f, -0.20f, -0.40f); // Toppen

    // Metallmaterial
    auto tetra_mat = std::make_shared<Metal>(glm::vec3(0.9, 0.9, 0.9));

    // trianglarna som bygger upp formen
    objects.push_back(std::make_shared<Triangle>(v0, v1, v2, glm::vec3(1, 1, 0), tetra_mat)); // Botten
    objects.push_back(std::make_shared<Triangle>(v0, v3, v1, glm::vec3(1, 1, 0), tetra_mat)); // Baksidan
    objects.push_back(std::make_shared<Triangle>(v1, v3, v2, glm::vec3(1, 1, 0), tetra_mat)); // Höger framsida
    objects.push_back(std::make_shared<Triangle>(v2, v3, v0, glm::vec3(1, 1, 0), tetra_mat)); // Vänster framsida



}



bool Scene::intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    HitRecord temp_rec; // temp struct
    bool hit_anything = false;
    float closest_so_far = t_max; // vill bara veta om den närmaste träffen

    // Loopa igenom alla väggar i listan
    for (const auto& object : objects) {

        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t; // uppdatera avståndet så vi hittar det som är närmast
            rec = temp_rec; // kopierar alla data till den slutgiltiga structen
        }
    }
    return hit_anything;
}



// update 