#include "camera.hpp"
#include "group.hpp"
#include "image.hpp"
#include "path_tracer.hpp"
#include "random.hpp"
#include "scene.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

const int STEP_SIZE = 100;
const int MAX_SIZE = 2048;

Vector3f color_map[MAX_SIZE][MAX_SIZE];

int main(int argc, char *argv[]) {
    if (argc != 3)
        return 1;
    std::string scene_name = argv[1];
    std::string output_prefix = "output/" + scene_name;
    int num_samples = std::stoi(argv[2]);

    Scene world(scene_name);
    Camera *camera = world.getCamera();
    Group *group = world.getGroup();
    Image image(camera->getWidth(), camera->getHeight());
    Vector3f background_color = world.getBackgroundColor();
    std::random_device rd;

    int iter = 0;
    while (iter < num_samples) {
        printf("Running iteration %d\n", iter);
        int n = std::min(num_samples - iter, STEP_SIZE);

#pragma omp parallel for schedule(dynamic, 1)
        for (int x = 0; x < camera->getWidth(); ++x) {
            RandomGenerator gen(rd());
            PathTracer pt(group, background_color, gen);
            for (int y = 0; y < camera->getHeight(); ++y) {
                Vector3f color = Vector3f::ZERO;
                for (int i = 0; i < n; ++i) {
                    Ray ray = camera->generateRay(Vector2f(x, y) + gen.tent2f(), gen);
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

        std::string output_file = output_prefix + ".bmp";
        image.SaveImage(output_file.c_str());
    }

    return 0;
}
