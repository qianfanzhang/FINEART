#include "camera.hpp"
#include "group.hpp"
#include "image.hpp"
#include "scene_parser.hpp"
#include "path_tracing.hpp"
#include "random.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "Usage: ./bin/PA1 <input scene file> <output bmp file> <number of samples>" << std::endl;
        return 1;
    }
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int numSamples = std::stoi(argv[3]);

    SceneParser parser(inputFile.c_str());
    Camera *camera = parser.getCamera();
    Group *group = parser.getGroup();
    Image image(camera->getWidth(), camera->getHeight());
    Vector3f background_color = parser.getBackgroundColor();
    std::random_device rd;

#pragma omp parallel for schedule(dynamic, 1)
    for (int x = 0; x < camera->getWidth(); ++x) {
        RandomGenerator gen(rd());
        PathTracing pt(group, background_color, 6, gen);
        for (int y = 0; y < camera->getHeight(); ++y) {
            Vector3f color = Vector3f::ZERO;
            for (int i = 0; i < numSamples; ++i) {
                // Ray ray = camera->generateRay(Vector2f(x, y));
                Ray ray = camera->generateRay(Vector2f(x, y) + gen.tent2f());
                color += pt.getRadiance(ray) * (1. / numSamples);
            }
            image.SetPixel(x, y, color);
        }
    }

    image.SaveImage(outputFile.c_str());

    return 0;
}