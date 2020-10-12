#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "hit.hpp"
#include "ray.hpp"
#include <iostream>

static float clamp(float x) {
    return x > 1 ? 1 : (x < 0 ? 0 : x);
}

class Material {
public:
    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0)
        : diffuseColor(d_color), specularColor(s_color), shininess(s) {}
    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    Vector3f shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;
        Vector3f L = dirToLight.normalized();
        Vector3f N = hit.getNormal().normalized();
        Vector3f V = -ray.getDirection().normalized();
        Vector3f R = (2 * Vector3f::dot(N, L) * N - L).normalized();
        shaded += diffuseColor * clamp(Vector3f::dot(L, N));
        shaded += specularColor * powf(clamp(Vector3f::dot(V, R)), shininess);
        return lightColor * shaded;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};

#endif