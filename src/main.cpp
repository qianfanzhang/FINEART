#include "camera.hpp"
#include "group.hpp"
#include "image.hpp"
#include "light.hpp"
#include "scene_parser.hpp"
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
    Group *baseGroup = parser.getGroup();
    Image image(camera->getWidth(), camera->getHeight());

    for (int x = 0; x < camera->getWidth(); ++x) {
        for (int y = 0; y < camera->getHeight(); ++y) {
            Vector3f color = Vector3f::ZERO;
            for (int i = 0; i < numSamples; ++i) {
                Ray ray = camera->generateRay(Vector2f(x + Random::tent(), y + Random::tent()));
                
                // Hit hit;
                // if (baseGroup->intersect(ray, hit, 0)) {
                //     Vector3f finalColor = Vector3f::ZERO;
                //     for (int li = 0; li < parser.getNumLights(); ++li) {
                //         Light *light = parser.getLight(li);
                //         Vector3f L, lightColor;
                //         light->getIllumination(ray.pointAtParameter(hit.getT()), L, lightColor);
                //         color += hit.getMaterial()->shade(ray, hit, L, lightColor) * (1. / numSamples);
                //     }
                // }
            }
            image.SetPixel(x, y, color);
            // image.SetPixel(x, y, parser.getBackgroundColor());
        }
    }

    image.SaveImage(outputFile.c_str());

    return 0;
}