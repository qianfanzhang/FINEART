#define TINYEXR_IMPLEMENTATION
#include "tinyexr.hpp"

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

Vector3f color_map[Utils::MAX_RESOLUTION][Utils::MAX_RESOLUTION];

int main(int argc, char *argv[]) {
    auto program_start = std::chrono::system_clock::now();
    std::cout << "[main] BINART start" << std::endl;

    if (argc != 4) {
        throw std::invalid_argument("too few arguments");
    }

    std::string scene_name = argv[1];
    std::string output_name = "output/" + scene_name + ".png";
    std::string checkpoint_name = "checkpoint/" + scene_name + std::to_string(time(nullptr)) + ".ckpt";
    int num_samples = std::stoi(argv[2]);
    int resolution = std::stoi(argv[3]);

    Scene world(scene_name, resolution);

    Camera *camera = world.getCamera();
    Group *group = world.getGroup();
    Medium *medium = world.getMedium();
    Medium *start_medium = world.getStartMedium();
    SkyLight *sky_light = world.getSkyLight();
    std::random_device rd;

    assert(camera->getWidth() < Utils::MAX_RESOLUTION);
    assert(camera->getHeight() < Utils::MAX_RESOLUTION);

    std::cout << "[main] start rendering " << scene_name << " with sample=" << num_samples << ", resolution=" << resolution << std::endl;
    PathTracer::debug();

    int iter = 0;
    while (iter < num_samples) {
        auto iteration_start = std::chrono::system_clock::now();

        int n = std::min(num_samples - iter, STEP_SIZE);

#pragma omp parallel for schedule(dynamic, 1)
        for (int x = 0; x < camera->getWidth(); ++x) {
            RandomGenerator gen(rd());
            PathTracer pt(group, medium, sky_light, gen);
            for (int y = 0; y < camera->getHeight(); ++y) {
                Vector3f color = Vector3f::ZERO;
                for (int i = 0; i < n; ++i) {
                    Ray ray = camera->generateRay(Vector2f(x, y) + gen.tent2f(), gen);
                    color += Utils::clamp(pt.getRadiance(ray, start_medium), 100);
                }
                color_map[x][y] += color;
            }
        }

        iter += n;
        Utils::save_image(iter, camera->getWidth(), camera->getHeight(), color_map, output_name);
        Utils::save_checkpoint(iter, camera->getWidth(), camera->getHeight(), color_map, checkpoint_name);

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
