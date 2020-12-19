#include "path_tracer.hpp"
#include "hit.hpp"
#include "object/object3d.hpp"
#include "random.hpp"
#include "vecmath.h"
#include <iostream>

Vector3f PathTracer::getRadiance(Ray ray, int depth) {
    if (depth > MAX_DEPTH)
        return Vector3f::ZERO;

    // scene intersection
    Hit hit;
    Vector3f beta(1, 1, 1);
    bool has_intersection = this->group->intersect(ray, hit);

    // medium interaction
    Ray medium_ray = ray;
    bool has_medium_interaction = this->medium != nullptr && this->medium->interact(medium_ray, beta, this->gen, hit.t);
    if (!has_intersection && !has_medium_interaction)
        return Vector3f::ZERO;
    if (beta.length() < EPS)
        return Vector3f::ZERO;
    if (has_medium_interaction) {
        Vector3f L = beta * group->sampleAllLights(ray, hit, nullptr, this->medium, this->gen);
        return L + beta * (getRadiance(medium_ray, depth + 1));
    }

    // initialize helper variables
    Object3D *object = hit.getObject3D();
    assert(object != nullptr);
    Material *material = object->getMaterial();
    assert(material != nullptr);
    Vector3f emission = material->getEmission();
    Vector3f color = material->getColor(hit.uv);

    // direct lighting (unifrom sample all lights)
    Vector3f L = beta * emission;
    if ((beta * color).length() < EPS)
        return L;
    L += beta * color * group->sampleAllLights(ray, hit, material, nullptr, this->gen);

    // russian roulette
    float max_color = std::max(color.x(), std::max(color.y(), color.z()));
    if (depth >= MIN_DEPTH) {
        if (gen.uniform() < max_color)
            color = color / max_color;
        else
            return L;
    }

    // surface interaction
    beta *= material->sampleRay(ray, hit, this->gen);
    L += beta * color * getRadiance(ray, depth + 1);

    return L;
}