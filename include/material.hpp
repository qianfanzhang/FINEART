#ifndef MY_MATERIAL_H
#define MY_MATERIAL_H

#include "random.hpp"
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
    Material()
        : type(DIFFUSE), diffuse(1, 1, 1), emission(0, 0, 0), texture(nullptr) {}

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

    float sampleRay(Ray &ray, const Hit &hit, RandomGenerator &gen) const {
        Vector3f point = ray.pointAtParameter(hit.getT());
        Vector3f direction = ray.getDirection();
        Vector3f normal = hit.getNormal();
        Vector3f normal0 = Vector3f::dot(direction, normal) < 0 ? normal : -normal;

        if (type == DIFFUSE) {
            Vector3f next_d = gen.uniformOnHemisphere(normal);
            ray = Ray(point, next_d);
            return 1;

        } else if (type == SPECULAR) {
            Vector3f next_d = (direction - 2 * Vector3f::dot(normal, direction) * normal).normalized();
            ray = Ray(point, next_d);
            return 1;

        } else if (type == REFRACTIVE) {
            Vector3f refl_d = (direction - 2 * Vector3f::dot(normal, direction) * normal).normalized();
            Ray refl_ray(point, refl_d);

            bool into = Vector3f::dot(normal, normal0) > 0;
            float nc = 1, nt = 1.5;
            float nnt = into ? nc / nt : nt / nc;
            float ddn = Vector3f::dot(direction, normal0);
            float cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

            // total internal reflection
            if (cos2t < 0) {
                ray = refl_ray;
                return 1;
            }

            // otherwise, choose reflection or refraction
            Vector3f refr_d = (nnt * direction - normal * ((into ? 1 : -1) * (ddn * nnt + std::sqrt(cos2t)))).normalized();
            Ray refr_ray(point, refr_d);

            float a = nt - nc, b = nt + nc;
            float R0 = a * a / (b * b);
            float c = 1 - (into ? -ddn : Vector3f::dot(refr_d, normal));
            float Re = R0 + (1 - R0) * c * c * c * c * c;
            float Tr = 1 - Re;
            float P = 0.25 + 0.5 * Re;
            if (gen.uniform() < P) {
                ray = refl_ray;
                return Re / P;
            } else {
                ray = refr_ray;
                return Tr / (1 - P);
            }
        } else
            abort();
    }

    MaterialType type;
    Vector3f diffuse;
    Vector3f emission;
    Texture *texture;
};

#endif
