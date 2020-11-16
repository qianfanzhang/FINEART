#ifndef MY_FUCKING_RANDOM_H_NOT_YOUR_FUCKING_STD_RANDOM
#define MY_FUCKING_RANDOM_H_NOT_YOUR_FUCKING_STD_RANDOM

#include "utils.hpp"
#include "vecmath.h"
#include <cassert>
#include <cmath>
#include <numbers>
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

    inline Vector3f uniformOnHemisphere(const Vector3f &w) {
        Vector3f u = Utils::getUAxisGivenNormal(w);
        Vector3f v = Utils::getVAxisGivenNormal(w, u);
        float theta = 2 * std::numbers::pi * uniform();
        float r2 = uniform(), r = std::sqrt(r2); // length projected on uv-plane
        Vector3f d = (u * std::cos(theta) * r + v * std::sin(theta) * r + w * std::sqrt(1 - r2)).normalized();
        return d;
    }
};

#endif
