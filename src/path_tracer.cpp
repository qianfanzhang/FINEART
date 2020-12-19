#include "path_tracer.hpp"
#include "hit.hpp"
#include "object/object3d.hpp"
#include "random.hpp"
#include "vecmath.h"
#include <iostream>

Vector3f PathTracer::getRadiance(Ray ray, int depth) {
    if (depth > MAX_DEPTH)
        return Vector3f::ZERO;

    Hit hit;
    Vector3f beta(1, 1, 1);
    bool has_intersection = this->group->intersect(ray, hit);
    bool has_medium_interaction = this->medium != nullptr && this->medium->interact(ray, beta, this->gen, hit.getT());

    if (!has_intersection && !has_medium_interaction)
        return Vector3f::ZERO;
    if (has_medium_interaction) {
        return beta * getRadiance(ray, depth + 1);
    }

    assert(beta[0] >= 0 && beta[1] >= 0 && beta[2] >= 0);
    if (beta.length() < 1e-3)
       return Vector3f::ZERO;

    Object3D *object = hit.getObject3D();
    assert(object != nullptr);
    Material *material = object->getMaterial();
    assert(material != nullptr);
    Vector3f emission = material->getEmission();
    Vector3f color = material->getColor(hit.getUV());
    if (color.length() < 1e-3)
        return Vector3f::ZERO;

    float max_color = std::max(color.x(), std::max(color.y(), color.z()));
    if (depth >= MIN_DEPTH) {
        if (gen.uniform() < max_color)
            color = color / max_color;
        else
            return beta * emission;
    }

    beta *= material->sampleRay(ray, hit, this->gen);
    return beta * (emission + color * getRadiance(ray, depth + 1));
}