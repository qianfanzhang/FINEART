#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "scene.hpp"
#include "camera.hpp"
#include "group/basic_group.hpp"
#include "group/kd_tree.hpp"
#include "light.hpp"
#include "material.hpp"
#include "object/mesh.hpp"
#include "object/plane.hpp"
#include "object/sphere.hpp"
#include "vecmath.h"
#include <chrono>

using Mat4 = Matrix4f;
using Utils::rad;

Vector2f vec(float x, float y) {
    return Vector2f(x, y);
}

Vector3f vec(float x, float y, float z) {
    return Vector3f(x, y, z);
}

// The new objects will NOT be deleted until the program exists

Scene::Scene(const std::string &scene_name, int resolution) {
    auto start_time = std::chrono::system_clock::now();
    std::cout << "[Scene] start building " << scene_name << std::endl;

    medium = nullptr;
    start_medium = nullptr;
    sky_light = nullptr;

    group = new KdTree();

#include "scene/common_resources.hpp"
#include "scene/all_scenes.hpp"

    assert(camera != nullptr);

    group->build();
    group->debug();
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;
    std::cout << "[Scene] finished in " << diff.count() << " s" << std::endl;
}

#pragma GCC diagnostic pop