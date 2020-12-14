#ifndef MY_MATERIAL_H
#define MY_MATERIAL_H

#include "ray.hpp"
#include "texture.hpp"
#include "vecmath.h"
#include <cassert>
#include <iostream>
#include <utility>

enum MaterialType {
    DIFFUSE,
    SPECULAR,
    REFRACTIVE
};

class Material {
public:
    Material(MaterialType type, Vector3f diffuse, Vector3f emission = Vector3f::ZERO)
        : type(type), diffuse(diffuse), emission(emission), texture(nullptr) {}

    Material(MaterialType type, Texture *texture)
        : type(type), diffuse({1, 1, 1}), emission(Vector3f::ZERO), texture(texture) {}

    virtual ~Material() = default;

    MaterialType getType() const {
        return type;
    }

    Vector3f getEmission() const {
        return emission;
    }

    Vector3f getColor(const Vector2f &uv) const {
        if (texture == nullptr)
            return diffuse;
        else
            return diffuse * texture->getColor(uv);
    }

    MaterialType type;
    Vector3f diffuse;
    Vector3f emission;
    Texture *texture;
};

#endif
