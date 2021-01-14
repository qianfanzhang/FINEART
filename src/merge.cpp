#include "utils.hpp"
#include "vecmath.h"
#include <filesystem>
#include <ctime>
#include <iostream>

Vector3f rgb[Utils::MAX_RESOLUTION][Utils::MAX_RESOLUTION];

int main() {
    int num_samples = 0;
    int width = 0, height = 0;

    for (const auto &entry : std::filesystem::recursive_directory_iterator("checkpoint")) {
        Utils::load_checkpoint(num_samples, width, height, rgb, entry.path().string());
    }

    Utils::save_image(num_samples, width, height, rgb, "output/merge" + std::to_string(time(nullptr)) + ".png");
}