#ifndef _MY_FUCKING_RANDOM_H_NOT_YOUR_FUCKING_STD_RANDOM
#define _MY_FUCKING_RANDOM_H_NOT_YOUR_FUCKING_STD_RANDOM

#include <cassert>
#include <cmath>
#include <random>
#include <numbers>
#include <vector>
#include <omp.h>
#include <vecmath.h>

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
        Vector3f u = Vector3f::cross(std::abs(w.x()) > 0.1 ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0), w).normalized();
        Vector3f v = Vector3f::cross(w, u);
        float theta = 2 * std::numbers::pi * uniform();
        float r2 = uniform(), r = std::sqrt(r2); // length projected on uv-plane
        Vector3f d =  (u * std::cos(theta) * r + v * std::sin(theta) * r + w * std::sqrt(1 - r2)).normalized();
        return d;
    }
};

#endif
