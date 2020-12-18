#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "scene.hpp"
#include "camera.hpp"
#include "group/basic_group.hpp"
#include "group/kd_tree.hpp"
#include "material.hpp"
#include "object/mesh.hpp"
#include "object/plane.hpp"
#include "object/sphere.hpp"
#include "vecmath.h"
#include <chrono>

using Mat4 = Matrix4f;

Vector2f vec(float x, float y) {
    return Vector2f(x, y);
}

Vector3f vec(float x, float y, float z) {
    return Vector3f(x, y, z);
}

float rad(float deg) {
    return deg / 180 * Utils::pi;
}

// The new objects will NOT be deleted until the program exists

Scene::Scene(const std::string &scene_name, int resolution) {
    auto start_time = std::chrono::system_clock::now();
    std::cout << "[Scene] start building " << scene_name << std::endl;

    background_color = vec(0, 0, 0);

    group = new KdTree();

#include "scene/common_resources.hpp"

    if (scene_name == "earth_in_box") {
#include "scene/earth_in_box.hpp"
    } else if (scene_name == "dragon_in_box") {
#include "scene/dragon_in_box.hpp"
    } else if (scene_name == "girl_in_box") {
#include "scene/girl_in_box.hpp"
    } else if (scene_name == "house") {
#include "scene/house.hpp"
    } else if (scene_name == "glass") {
#include "scene/glass.hpp"
    } else
        throw std::invalid_argument("scene_name not found");

    assert(camera != nullptr);

    group->build();
    group->debug();
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;
    std::cout << "[Scene] finished in " << diff.count() << " s" << std::endl;
}

#pragma GCC diagnostic pop