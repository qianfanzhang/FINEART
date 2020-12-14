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

    double uniform() {
        std::uniform_real_distribution<> dist(0, 1);
        return dist(e);
    }

    double tent() {
        double r = 2 * uniform();
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
        float theta = 2 * Utils::pi * uniform();
        float r2 = uniform(), r = std::sqrt(r2); // length projected on uv-plane
        Vector3f d = (u * std::cos(theta) * r + v * std::sin(theta) * r + w * std::sqrt(1 - r2)).normalized();
        return d;
    }

    /*
    float henyeyGreenstein(const Vector3f &wo, Vector3f *wi) const {
        // Compute $\cos \theta$ for Henyey--Greenstein sample
        float cosTheta;
        if (std::abs(g) < 1e-3)
            cosTheta = 1 - 2 * u[0];
        else {
            float sqrTerm = (1 - g * g) / (1 + g - 2 * g * u[0]);
            cosTheta = -(1 + g * g - sqrTerm * sqrTerm) / (2 * g);
        }

        // Compute direction _wi_ for Henyey--Greenstein sample
        float sinTheta = std::sqrt(std::max((Float)0, 1 - cosTheta * cosTheta));
        float phi = 2 * Pi * u[1];
        Vector3f v1, v2;
        CoordinateSystem(wo, &v1, &v2);
        *wi = SphericalDirection(sinTheta, cosTheta, phi, v1, v2, wo);
        return PhaseHG(cosTheta, g);
    }
    */
};

#endif
