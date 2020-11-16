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
    Material(MaterialType type, Texture *texture)
        : type(type), texture(texture), emission(Vector3f::ZERO) {}

    Material(MaterialType type, Texture *texture, Vector3f emission)
        : type(type), texture(texture), emission(emission) {}

    virtual ~Material() = default;

    MaterialType getType() const {
        return type;
    }

    Texture *getTexture() const {
        return texture;
    }

    const Vector3f &getEmission() const {
        return emission;
    }

protected:
    MaterialType type;
    Texture *texture;
    Vector3f emission;
};

#endif
