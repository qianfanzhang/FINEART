#ifndef MY_LIGHT_H
#define MY_LIGHT_H

#include "group/group.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"

class Light {
public:
    virtual ~Light() {}

    virtual Vector3f sampleRay(const Vector3f &point, Vector3f &dir, float &t, RandomGenerator &gen) = 0;
};

class PointLight : public Light {
public:    
    PointLight(const Vector3f &pos, const Vector3f &intensity)
        : pos(pos), intensity(intensity) {}

    Vector3f sampleRay(const Vector3f &point, Vector3f &dir, float &t, RandomGenerator &gen __attribute__((unused))) override {
        dir = pos - point;
        float d2 = dir.squaredLength();
        t = std::sqrt(d2);
        dir = dir / t; // normalized
        return intensity / d2;
    }

private:
    Vector3f pos;
    Vector3f intensity;
};

class SpotLight : public Light {
public:
    SpotLight(const Vector3f &pos, const Vector3f &normal, float width, float falloff, const Vector3f &intensity)
        : pos(pos), normal(normal.normalized()), 
        cos_width(std::cos(Utils::rad(width))), cos_falloff(std::cos(Utils::rad(falloff))), 
        intensity(intensity) {}

    Vector3f sampleRay(const Vector3f &point, Vector3f &dir, float &t, RandomGenerator &gen __attribute__((unused))) override {
        dir = pos - point;
        float d2 = dir.squaredLength();
        t = std::sqrt(d2);
        dir = dir / t; // normalized

        float cos_theta = Vector3f::dot(-dir, normal);
        if (cos_theta < cos_width)
            return Vector3f::ZERO;
        if (cos_theta > cos_falloff) {
            // printf("%.3f\n", (intensity / d2).x());
            return intensity / d2;
        }
        float delta = (cos_theta - cos_width) / (cos_falloff - cos_width);
        return intensity * (delta * delta) * (delta * delta) / d2;
    }

private:
    Vector3f pos;
    Vector3f normal;
    float cos_width;
    float cos_falloff;
    Vector3f intensity;
};

#endif