#include "path_tracer.hpp"
#include "hit.hpp"
#include "object/object3d.hpp"
#include "random.hpp"
#include "vecmath.h"
#include <iostream>

Vector3f PathTracer::getRadiance(Ray ray, int depth, Medium *medium) {
    if (depth > MAX_DEPTH)
        return Vector3f::ZERO;

    // medium = nullptr;

    // scene intersection
    Hit hit;
    Vector3f beta(1, 1, 1);
    bool has_intersection = this->group->intersect(ray, hit);

    Ray medium_ray = ray;
    bool has_medium_interaction = medium != nullptr && medium->interact(medium_ray, beta, this->gen, hit.t);

    if (beta.length() < EPS)
        return Vector3f::ZERO;

    if (has_medium_interaction) {
        // medium interaction
        Vector3f L = beta * group->sampleAllLights(ray, hit, nullptr, medium, this->gen);
        return L + beta * (getRadiance(medium_ray, depth + 1, medium));
    } else if (hit.object == group->world_bound) {
        // sky light
        return this->sky_light ? beta * this->sky_light->sampleRay(ray.origin, ray.direction, hit.t, gen) : Vector3f::ZERO;
    } else if (!has_intersection)
        return Vector3f::ZERO;

    // initialize helper variables
    Object3D *object = hit.getObject3D();
    assert(object != nullptr);
    Material *material = object->getMaterial();
    assert(material != nullptr);
    Medium *material_medium = object->getMedium();
    Vector3f emission = material->getEmission();
    Vector3f color = material->getColor(hit.uv);

    // direct lighting (unifrom sample all lights)
    Vector3f L = beta * emission;
    if ((beta * color).length() < EPS)
        return L;
    L += beta * color * group->sampleAllLights(ray, hit, material, medium, this->gen);

    // russian roulette
    float max_color = std::max(color.x(), std::max(color.y(), color.z()));
    if (depth >= MIN_DEPTH) {
        if (gen.uniform() < max_color)
            color = color / max_color;
        else
            return L;
    }

    // surface interaction
    bool inside = false;
    beta *= material->sampleRay(ray, hit, this->gen, inside);

    L += beta * color * getRadiance(ray, depth + 1, inside ? material_medium : this->medium);

    return L;
}