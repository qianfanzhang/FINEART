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

Scene::Scene(const std::string &scene_name) {
    if (scene_name == "01_earth_in_box") {
        group = new KdTree();

        camera = new ThinLenCamera(
            vec(0, 0.7, 4),   // center
            vec(0, -0.4, -1), // direction
            vec(0, 1, 0),     // up
            rad(50),          // angle
            0,                // aperture
            8.5,              // focus dist
            1000,             // width
            1000              // height
        );

        background_color = vec(0, 0, 0);

        Texture *white = add(new Texture(vec(1, 1, 1)));
        Texture *red = add(new Texture(vec(0.75, 0.25, 0.25)));
        Texture *green = add(new Texture(vec(0.25, 0.75, 0.25)));
        Texture *blue = add(new Texture(vec(0.25, 0.25, 0.75)));
        Texture *grey = add(new Texture(vec(0.5, 0.5, 0.5)));
        Texture *earth = add(new Texture("texture/earth.tga"));
        Texture *wood = add(new Texture("texture/wood.tga"));
        Texture *marble = add(new Texture("texture/marble.tga"));

        Material *light = add(new Material(DIFFUSE, white, vec(15, 15, 15)));
        Material *light_red = add(new Material(DIFFUSE, red, vec(3, 0, 0)));
        Material *light_green = add(new Material(DIFFUSE, green, vec(0, 3, 0)));
        Material *light_blue = add(new Material(DIFFUSE, blue, vec(0, 0, 3)));
        Material *diff_white = add(new Material(DIFFUSE, white));
        Material *diff_red = add(new Material(DIFFUSE, red));
        Material *diff_green = add(new Material(DIFFUSE, green));
        Material *diff_blue = add(new Material(DIFFUSE, blue));
        Material *diff_grey = add(new Material(DIFFUSE, grey));
        Material *diff_earth = add(new Material(DIFFUSE, earth));
        Material *spec = add(new Material(SPECULAR, white));
        Material *refr = add(new Material(REFRACTIVE, white));
        Material *refr_red = add(new Material(REFRACTIVE, red));
        Material *refr_green = add(new Material(REFRACTIVE, green));

        group->add(new Sphere(vec(0, 3.5, 0.75), 1.82, light));
        // group->add(new Sphere(vec(-1.6, 0, -1.6), 0.2, light_red));
        // group->add(new Sphere(vec(-1.1, 0, -1.3), 0.2, light_green));
        // group->add(new Sphere(vec(-0.6, 0, -1.0), 0.2, light_blue));

        // group->add(new Transform(Mat4::translation(0.2, -1, -0.75) * Mat4::rotateY(2.61) * Mat4::rotateX(-1.57),
        //                                new Sphere(vec(0, 0, 0), 0.75, diff_earth)));
        // group->add(new Sphere(vec(-0.75, -0.6, 1.5), 0.6, refr));

        group->add(new Plane(vec(0, 0, 1), -2, diff_grey));
        group->add(new Plane(vec(0, 1, 0), -2, diff_grey));
        group->add(new Plane(vec(0, 1, 0), 2, diff_grey));
        group->add(new Plane(vec(1, 0, 0), -2, diff_red));
        group->add(new Plane(vec(1, 0, 0), 2, diff_blue));
        group->add(new Mesh("mesh/fixed.perfect.dragon.100K.obj", refr_green, Matrix4f::translation(0.5, -1.35, 1.1) * Matrix4f::uniformScaling(1.5)));
        group->add(new Mesh("mesh/bunny.fine.obj", diff_red, Matrix4f::translation(-0.5, -2.35, -0.5) * Matrix4f::uniformScaling(10)));
        group->add(new Plane(vec(0, 1, 0), -0.7, refr));

    } else {
        abort();
    }

    group->build();
    // group->debug();
    // fflush(stdout);
}

#pragma GCC diagnostic pop