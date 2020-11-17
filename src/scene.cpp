#include "scene.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "transform.hpp"
#include "triangle.hpp"
#include "vecmath.h"
#include <numbers>

using Mat4 = Matrix4f;

Vector2f vec(float x, float y) {
    return Vector2f(x, y);
}

Vector3f vec(float x, float y, float z) {
    return Vector3f(x, y, z);
}

float rad(float deg) {
    return deg / 180 * std::numbers::pi;
}

Scene::Scene(const std::string &scene_name) {
    if (scene_name == "01_earth_in_box") {
        camera = new ThinLenCamera(
            vec(0, 0, 10), // center
            vec(0, 0, -1), // direction
            vec(0, 1, 0),  // up
            rad(30),       // angle
            0.25,          // aperture
            8.5,           // focus dist
            1000,          // width
            1000           // height
        );

        background_color = vec(0, 0, 0);

        Texture *white = add(new Texture(vec(1, 1, 1)));
        Texture *red = add(new Texture(vec(0.75, 0.25, 0.25)));
        Texture *green = add(new Texture(vec(0.25, 0.75, 0.75)));
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
        Material *diff_blue = add(new Material(DIFFUSE, blue));
        Material *diff_grey = add(new Material(DIFFUSE, grey));
        Material *diff_earth = add(new Material(DIFFUSE, earth));
        Material *spec = add(new Material(SPECULAR, white));
        Material *refr = add(new Material(REFRACTIVE, white));

        group = new Group();

        group->addObject(new Sphere(vec(0, 3.5, 0.75), 1.82, light));
        group->addObject(new Sphere(vec(-1.6, 0, -1.6), 0.2, light_red));
        group->addObject(new Sphere(vec(-1.1, 0, -1.3), 0.2, light_green));
        group->addObject(new Sphere(vec(-0.6, 0, -1.0), 0.2, light_blue));

        group->addObject(new Transform(Mat4::translation(0.2, -1, -0.75) * Mat4::rotateY(2.61) * Mat4::rotateX(-1.57),
                                       new Sphere(vec(0, 0, 0), 0.75, diff_earth)));
        group->addObject(new Sphere(vec(-0.75, -0.6, 1.5), 0.6, refr));

        group->addObject(new Plane(vec(0, 0, 1), -2, diff_grey));
        group->addObject(new Plane(vec(0, 1, 0), -2, diff_grey));
        group->addObject(new Plane(vec(0, 1, 0), 2, diff_grey));
        group->addObject(new Plane(vec(1, 0, 0), -2, diff_red));
        group->addObject(new Plane(vec(1, 0, 0), 2, diff_blue));
        // group->addObject(new Transform(Matrix4f::translation(-1.5, -2, 1.2) * Matrix4f::rotateZ(0.5) * Matrix4f::rotateY(1.57),
        //                                new Mesh("mesh/rectangle.obj", spec)));
    } else {
        abort();
    }
}