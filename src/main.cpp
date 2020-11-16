#include "camera.hpp"
#include "group.hpp"
#include "image.hpp"
#include "path_tracing.hpp"
#include "random.hpp"
#include "scene_parser.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

const size_t MAX_SIZE = 2048;

Vector3f color_map[MAX_SIZE][MAX_SIZE];

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

    int iter = 0;
    while (iter < numSamples) {
        printf("Iteration %d\n", iter);
        int n = std::min(numSamples - iter, 100);

#pragma omp parallel for schedule(dynamic, 1)
        for (int x = 0; x < camera->getWidth(); ++x) {
            RandomGenerator gen(rd());
            PathTracing pt(group, background_color, 13, gen);
            for (int y = 0; y < camera->getHeight(); ++y) {
                Vector3f color = Vector3f::ZERO;
                for (int i = 0; i < n; ++i) {
                    Ray ray = camera->generateRay(Vector2f(x, y) + gen.tent2f());
                    color += pt.getRadiance(ray);
                }
                color_map[x][y] += color;
            }
        }

        iter += n;
        for (int x = 0; x < camera->getWidth(); ++x) {
            for (int y = 0; y < camera->getHeight(); ++y) {
                image.SetPixel(x, y, color_map[x][y] / iter);
            }
        }
        image.SaveImage(outputFile.c_str());
    }

    return 0;
}
