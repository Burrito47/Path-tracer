#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include "scene.h"
#include "camera.h"



int main() {

    Scene scene;

    scene.addRoom();

    Camera camera(256, 256);

    auto start = std::chrono::high_resolution_clock::now();

    camera.render("image.ppm", scene);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time to complete render in " << elapsed.count() << " seconds." << std::endl;

    return 0;



    // Render

  




}