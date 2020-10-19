#ifndef MATERIAL_H
#define MATERIAL_H

#include "hit.hpp"
#include "ray.hpp"
#include <vecmath.h>
#include <cassert>
#include <iostream>

enum MaterialType {
    DIFFUSE, SPECULAR, REFRACTIVE
};

class Material {
public:
    Vector3f color;
    Vector3f emission;
    MaterialType type;

    explicit Material(const Vector3f &color, const Vector3f &emission, MaterialType type)
        : color(color), emission(emission), type(type) {}
    virtual ~Material() = default;
};

#endif
