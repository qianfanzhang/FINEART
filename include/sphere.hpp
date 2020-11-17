#ifndef MY_SPHERE_H
#define MY_SPHERE_H

#include "hit.hpp"
#include "object3d.hpp"
#include "vecmath.h"
#include <cmath>
#include <cstdlib>
#include <numbers>

class Sphere : public Object3D {
public:
    Sphere() = delete;

    Sphere(const Vector3f &center, float radius, Material *material)
        : Object3D(material), center(center), radius(radius) {}

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f oc = center - r.getOrigin();
        float a = r.getDirection().squaredLength();
        float b = Vector3f::dot(oc, r.getDirection());
        float det = b * b - (oc.squaredLength() - radius * radius) * a;

        if (det > 0) {
            det = std::sqrt(det);
            float t;
            if (((t = (b - det) / a) >= tmin || (t = (b + det) / a) >= tmin) && h.getT() > t) {
                h = Hit(t, this, r.pointAtParameter(t) - center);
                return true;
            }
        }
        return false;
    }

    Vector2f getUVPoint(const Vector3f &point) const {
        Vector3f d = (point - center).normalized();
        float phi = std::atan2(d.y(), d.x());
        float theta = std::asin(d.z());
        float u = 0.5 + phi / (2 * std::numbers::pi);
        float v = 0.5 + theta / std::numbers::pi;
        return Vector2f(u, v);
    }

private:
    Vector3f center;
    float radius;
};

#endif