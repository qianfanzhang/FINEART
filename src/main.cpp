#include "camera.hpp"
#include "group/group.hpp"
#include "image.hpp"
#include "path_tracer.hpp"
#include "random.hpp"
#include "scene.hpp"
#include <cassert>
#include <chrono>
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
    auto program_start = std::chrono::system_clock::now();
    std::cout << "[main] BINART start" << std::endl;

    if (argc != 4) {
        throw std::invalid_argument("too few arguments");
    }

    std::string scene_name = argv[1];
    std::string output_prefix = "output/" + scene_name;
    int num_samples = std::stoi(argv[2]);
    int resolution = std::stoi(argv[3]);

    Scene world(scene_name, resolution);
    Camera *camera = world.getCamera();
    Group *group = world.getGroup();
    Image image(camera->getWidth(), camera->getHeight());
    Vector3f background_color = world.getBackgroundColor();
    std::random_device rd;

    std::cout << "[main] start rendering " << scene_name << " with sample=" << num_samples << ", resolution=" << resolution << std::endl;
    PathTracer::debug();

    int iter = 0;
    while (iter < num_samples) {
        auto iteration_start = std::chrono::system_clock::now();

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
                image.set(x, y, color_map[x][y] / iter);
            }
        }

        std::string output_file = output_prefix + ".png";
        image.save(output_file.c_str());

        auto iteration_end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = iteration_end - iteration_start;
        std::cout << "[main] finished iteration " << iter << "/" << num_samples << " in " << diff.count() << " s "
                  << "(" << diff.count() / n << " s/sample, eta " << (num_samples - iter) * diff.count() / n / 60 << " min)" << std::endl;
    }

    auto program_end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = program_end - program_start;
    std::cout << "[main] BINART ended in " << diff.count() / 60 << " min" << std::endl;

    return 0;
}
