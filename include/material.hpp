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
    explicit Material(const Vector3f &emission, MaterialType type, Texture &&texture)
        : emission(emission), type(type), texture(std::move(texture)) {}

    virtual ~Material() = default;

    const Vector3f &getEmission() const {
        return emission;
    }

    MaterialType getType() const {
        return type;
    }

    const Texture &getTexture() const {
        return texture;
    }

protected:
    Vector3f emission;
    MaterialType type;
    Texture texture;
};

#endif
