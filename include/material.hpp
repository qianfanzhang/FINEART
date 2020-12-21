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

class BSDF {
public:
    virtual ~BSDF() = default;

    virtual float f(const Vector3f &wo, const Vector3f &wi, const Vector3f &normal) const = 0;

    virtual float sample(Vector3f &wo, const Vector3f &wi, const Vector3f &normal, RandomGenerator &gen) const = 0;

    virtual bool isDelta() const = 0;
};

class DiffuseBSDF : public BSDF {
public:
    DiffuseBSDF() = default;

    float f(const Vector3f &wo, const Vector3f &wi, const Vector3f &normal) const override {
        float u = Vector3f::dot(wo, normal);
        float v = Vector3f::dot(wi, normal);
        return (u >= 0 && v >= 0) || (u <= 0 && v <= 0) ? Utils::INV_PI : 0;
    }

    float sample(Vector3f &wo, const Vector3f &wi, const Vector3f &normal, RandomGenerator &gen) const override {
        Vector3f normal0 = Vector3f::dot(wi, normal) > 0 ? normal : -normal;
        wo = gen.uniformOnHemisphere(normal0);
        return 1;
    }

    bool isDelta() const override {
        return false;
    }
};

class SpecularBSDF : public BSDF {
public:
    SpecularBSDF() = default;

    float f(const Vector3f &wo, const Vector3f &wi, const Vector3f &normal) const override {
        return 0;
    }

    float sample(Vector3f &wo, const Vector3f &wi, const Vector3f &normal, RandomGenerator &gen) const override {
        wo = (2 * Vector3f::dot(wi, normal) * normal - wi).normalized();
        return 1;
    }

    bool isDelta() const override {
        return true;
    }
};

class RefractiveBSDF : public BSDF {
public:
    RefractiveBSDF(float nt) : nt(nt) {}

    float f(const Vector3f &wo, const Vector3f &wi, const Vector3f &normal) const override {
        return 0;
    }

    float sample(Vector3f &wo, const Vector3f &wi, const Vector3f &normal, RandomGenerator &gen) const override {
        Vector3f refl_d = (2 * Vector3f::dot(wi, normal) * normal - wi).normalized();

        bool into = Vector3f::dot(wi, normal) >= 0;
        float nc = 1;
        float nnt = into ? nc / nt : nt / nc;
        float ddn = -std::abs(Vector3f::dot(wi, normal));
        float cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        // total internal reflection
        if (cos2t < 0) {
            wo = refl_d;
            return 1;
        }

        // otherwise, choose either reflection or refraction
        Vector3f refr_d = (-nnt * wi - normal * ((into ? 1 : -1) * (ddn * nnt + std::sqrt(cos2t)))).normalized();

        float a = nt - nc, b = nt + nc;
        float R0 = a * a / (b * b);
        float c = 1 - (into ? -ddn : Vector3f::dot(refr_d, normal));
        float Re = R0 + (1 - R0) * c * c * c * c * c;
        float Tr = 1 - Re;
        float P = 0.25 + 0.5 * Re;

        if (gen.uniform() < P) {
            wo = refl_d;
            return Re / P;
        } else {
            wo = refr_d;
            return Tr / (1 - P);
        }
    }

    bool isDelta() const override {
        return true;
    }

private:
    float nt;
};

class Material {
public:
    Material(const std::vector<std::pair<BSDF *, float>> &bsdfs, const Vector3f &diffuse, const Vector3f &emission, Texture *texture)
        : bsdfs(bsdfs), diffuse(diffuse), emission(emission), texture(texture) {
        float sum = 0;
        for (auto &bsdf_pair : bsdfs) {
            if (bsdf_pair.first == nullptr) {
                std::cout << "[Material] error: bsdf nullptr" << std::endl;
                abort();
            }
            sum += bsdf_pair.second;
        }
        if (sum != 1) {
            std::cout << "[Material] error: probablity sum != 1" << std::endl;
            abort();
        }
    }

    Material(const std::vector<std::pair<BSDF *, float>> &bsdfs, const Vector3f &diffuse)
        : Material(bsdfs, diffuse, Vector3f::ZERO, nullptr) {}

    Material(const std::vector<std::pair<BSDF *, float>> &bsdfs, const Vector3f &diffuse, const Vector3f &emission)
        : Material(bsdfs, diffuse, emission, nullptr) {}

    Material(const std::vector<std::pair<BSDF *, float>> &bsdfs, Texture *texture)
        : Material(bsdfs, {1, 1, 1}, Vector3f::ZERO, texture) {}

    // Material(const Material &) = delete;

    // Material(Material &&material) {
    //     bsdfs = std::move(material.bsdfs);
    //     diffuse = material.diffuse;
    //     emission = material.emission;
    //     texture = material.texture;
    //     material.texture = nullptr;
    // }

    virtual ~Material() {
        // for (auto &bsdf_pair : bsdfs) {
        //     delete bsdf_pair.first;
        // }
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

    float sampleBSDF(BSDF *&bsdf, RandomGenerator &gen) {
        float p = gen.uniform();
        for (auto &bsdf_pair : bsdfs) {
            p -= bsdf_pair.second;
            if (p <= 0) {
                bsdf = bsdf_pair.first;
                return bsdf_pair.second;
            }
        }
        bsdf = bsdfs.back().first;
        return bsdfs.back().second;
    }

    float sampleRay(Ray &ray, const Hit &hit, RandomGenerator &gen) {
        BSDF *bsdf;
        float beta = sampleBSDF(bsdf, gen);

        Vector3f next_d;
        beta *= bsdf->sample(next_d, -ray.direction, hit.getNormal(), gen);
        ray = Ray(ray.pointAtParameter(hit.getT()), next_d);

        return beta;
    }

private:
    std::vector<std::pair<BSDF *, float>> bsdfs;
    Vector3f diffuse;
    Vector3f emission;
    Texture *texture;
};

#endif
