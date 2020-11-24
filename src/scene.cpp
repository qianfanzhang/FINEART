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
    if (scene_name == "01_earth_in_box") {
        group = new KdTree();

        auto center = vec(0, 0.4, 7);
        auto direction = vec(0, -0.17, -1);
        auto up = vec(0, 1, 0);
        float angle = rad(40);
        float aperture = 0;
        float focus_dist = 4.8;
        camera = new ThinLenCamera(center, direction, up, angle, aperture, focus_dist, resolution, resolution);

        background_color = vec(0, 0, 0);

        Texture *white = add(new Texture(vec(1, 1, 1)));
        Texture *red = add(new Texture(vec(0.75, 0.25, 0.25)));
        Texture *green = add(new Texture(vec(0.25, 0.75, 0.25)));
        Texture *blue = add(new Texture(vec(0.25, 0.25, 0.75)));
        Texture *grey = add(new Texture(vec(0.5, 0.5, 0.5)));
        Texture *earth = add(new Texture("texture/earth.png"));
        Texture *wood = add(new Texture("texture/wood.png"));
        Texture *marble = add(new Texture("texture/marble.png"));
        Texture *girl = add(new Texture("texture/girl.png"));

        Material *light = add(new Material(DIFFUSE, white, vec(15, 15, 15)));
        Material *light_white = add(new Material(DIFFUSE, red, vec(4, 4, 4)));
        Material *light_white0 = add(new Material(DIFFUSE, red, vec(3, 3, 3)));
        Material *light_red = add(new Material(DIFFUSE, red, vec(4, 0, 0)));
        Material *light_green = add(new Material(DIFFUSE, green, vec(0, 4, 0)));
        Material *light_blue = add(new Material(DIFFUSE, blue, vec(0, 0, 4)));
        Material *diff_white = add(new Material(DIFFUSE, white));
        Material *diff_red = add(new Material(DIFFUSE, red));
        Material *diff_green = add(new Material(DIFFUSE, green));
        Material *diff_blue = add(new Material(DIFFUSE, blue));
        Material *diff_grey = add(new Material(DIFFUSE, grey));
        Material *diff_earth = add(new Material(DIFFUSE, earth));
        Material *diff_girl = add(new Material(DIFFUSE, girl));
        Material *spec = add(new Material(SPECULAR, white));
        Material *refr = add(new Material(REFRACTIVE, white));
        Material *refr_red = add(new Material(REFRACTIVE, red));
        Material *refr_green = add(new Material(REFRACTIVE, green));

        group->add(new Sphere(vec(0, 3.5, 0.75), 1.82, light));

        group->add(new Sphere(vec(0, 0, -3), 1.9, light_white0));
        group->add(new Sphere(vec(3.5, 0, 3.75), 1.82, light_white));
        group->add(new Sphere(vec(-3.5, 0, 3.75), 1.82, light_white));

        // group->add(new Sphere(vec(-1, -1, -1), 0.5, diff_earth));
        // group->add(new Sphere(vec(-1, -1, 0), 0.5, diff_earth));
        // group->add(new Sphere(vec(-1, -1, 1), 0.5, diff_earth));
        // group->add(new Sphere(vec(-1, -1, 2), 0.5, diff_earth));
        // group->add(new Sphere(vec(-1, -1, 3), 0.5, diff_earth));

        // group->add(new Sphere(vec(-1.6, 0, -1.6), 0.2, light_red));
        // group->add(new Sphere(vec(-1.1, 0, -1.3), 0.2, light_green));
        // group->add(new Sphere(vec(-0.6, 0, -1.0), 0.2, light_blue));

        // group->add(new Sphere(vec(0.3, -1, -0.1), 0.75, diff_girl));
        // group->add(new Sphere(vec(-0.75, -0.6, 1.5), 0.6, refr));

        group->add(new Plane(vec(0, 0, 1), -2, diff_grey));
        group->add(new Plane(vec(0, 0, 1), -7.1, diff_grey));

        group->add(new Plane(vec(0, 1, 0), -2, diff_grey));
        group->add(new Plane(vec(0, 1, 0), 2, diff_grey));
        group->add(new Plane(vec(1, 0, 0), -2, diff_red));
        group->add(new Plane(vec(1, 0, 0), 2, diff_blue));
        // group->add(new Mesh("mesh/fixed.perfect.dragon.100K.obj", refr_green, Matrix4f::translation(0.5, -1, 1.1) * Matrix4f::uniformScaling(1.5)));
        // group->add(new Mesh("mesh/bunny.fine.obj", diff_red, Matrix4f::translation(-0.5, -2.35, -0.5) * Matrix4f::uniformScaling(10)));
        // group->add(new Plane(vec(0, 1, 0), -0.7, refr));

        group->add(new Mesh("mesh/girl.obj", diff_girl,
                            Matrix4f::translation(0, -2, 2) * Matrix4f::rotateX(rad(-90)) * Matrix4f::uniformScaling(0.0017),
                            false));

    } else {
        abort();
    }

    printf("Building group\n");
    fflush(stdout);
    group->build();
    // group->debug();
    // fflush(stdout);
}

#pragma GCC diagnostic pop