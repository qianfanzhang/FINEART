#ifndef MY_MEDIUM_H
#define MY_MEDIUM_H

#include "random.hpp"

// homogenous medium
class Medium {
public:
    Medium(const Vector3f &sigma_a, const Vector3f &sigma_s, float g)
        : sigma_a(sigma_a), sigma_s(sigma_s), sigma_t(sigma_a + sigma_s), g(g) {}

    float pdf(const Vector3f &wo, const Vector3f &wi) const {
        float cos_theta = Vector3f::dot(wo, wi);
        float t = 1 + g * g + 2 * g * cos_theta;
        return Utils::INV_4PI * (1 - g * g) / (t * std::sqrt(t));
    }

    bool interact(Ray &r, Vector3f &beta, RandomGenerator &gen, float &tmax) const {
        int c = gen.randint(0, 2); // channel
        float t = std::min(-std::log(1 - gen.uniform()) / sigma_t[c], tmax);
        // assert(t > 0);
        bool interacted = t < tmax;
        if (interacted) {
            Vector3f next_d;
            gen.henyeyGreenstein(-r.direction, next_d, g);
            r = Ray(r.pointAtParameter(t), next_d);
            // printf("interact %.2f %.2f {%.2f,%.2f,%.2f}\n", t, tmax, r.origin.x(), r.origin.y(), r.origin.z());
            tmax = t;
        }
        Vector3f tr(std::exp(-sigma_t[0] * t), std::exp(-sigma_t[1] * t), std::exp(-sigma_t[2] * t));
        Vector3f density = interacted ? (sigma_t * tr) : tr;
        float pdf = (density[0] + density[1] + density[2]) / 3;
        beta = interacted ? (tr * sigma_s / pdf) : (tr / pdf);
        return interacted;
    }

    Vector3f sigma_a;
    Vector3f sigma_s;
    Vector3f sigma_t;
    float g;
};

#endif