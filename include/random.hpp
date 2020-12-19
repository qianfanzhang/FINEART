#ifndef MY_FUCKING_RANDOM_H_NOT_YOUR_FUCKING_STD_RANDOM
#define MY_FUCKING_RANDOM_H_NOT_YOUR_FUCKING_STD_RANDOM

#include "utils.hpp"
#include "vecmath.h"
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

class RandomGenerator {
    std::default_random_engine e;

public:
    RandomGenerator(unsigned int seed)
        : e(seed) {}

    RandomGenerator(const RandomGenerator &) = delete;

    float uniform() {
        std::uniform_real_distribution<> dist(0, 1);
        return dist(e);
    }

    int randint(int l, int r) {
        std::uniform_int_distribution<> dist(l, r);
        return dist(e);
    }

    float tent() {
        float r = 2 * uniform();
        return r < 1 ? std::sqrt(r) - 1 : 1 - std::sqrt(2 - r);
    }

    Vector2f tent2f() {
        return Vector2f(tent(), tent());
    }

    Vector2f uniformInDisk() {
        for (;;) {
            Vector2f p(uniform() * 2 - 1, uniform() * 2 - 1);
            if (p.absSquared() >= 1)
                continue;
            return p;
        }
    }

    Vector3f uniformOnHemisphere(const Vector3f &w) {
        Vector3f u = Utils::getUAxisGivenNormal(w);
        Vector3f v = Utils::getVAxisGivenNormal(w, u);
        float theta = 2 * Utils::PI * uniform();
        float r2 = uniform(), r = std::sqrt(r2); // length projected on uv-plane
        Vector3f d = (u * std::cos(theta) * r + v * std::sin(theta) * r + w * std::sqrt(1 - r2)).normalized();
        return d;
    }

    // the function is adapt from pbrt-v3, https://github.com/mmp/pbrt-v3/tree/master/src/core/medium.cpp
    float henyeyGreenstein(const Vector3f &wo, Vector3f &wi, float g) {
        // compute cos(theta) for Henyey-Greenstein sample
        float cos_theta;
        if (std::abs(g) < 1e-3)
            cos_theta = 1 - 2 * uniform();
        else {
            float t = (1 - g * g) / (1 + g - 2 * g * uniform());
            cos_theta = -(1 + g * g - t * t) / (2 * g); // FIXME: positive or negative?
        }

        // compute wi for Henyey-Greenstein sample
        float sin_theta = std::sqrt(std::max((float)0, 1 - cos_theta * cos_theta));
        float phi = 2 * Utils::PI * uniform();
        Vector3f u = Utils::getUAxisGivenNormal(wo);
        Vector3f v = Utils::getVAxisGivenNormal(wo, u);
        wi = sin_theta * std::cos(phi) * u + sin_theta * std::sin(phi) * v + cos_theta * wo;

        // compute probability density
        float t = 1 + g * g + 2 * g * cos_theta;
        return Utils::INV_4PI * (1 - g * g) / (t * std::sqrt(t));
    }
};

#endif
