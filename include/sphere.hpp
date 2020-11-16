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
        float b = Vector3f::dot(oc, r.getDirection());
        float det = b * b - oc.squaredLength() + radius * radius;
        if (det > 0) {
            det = std::sqrt(det);
            float t;
            // printf("sphere intersect b=%.3f det=%.3f delta=%.3f\n", b, det, b - det);
            if (((t = b - det) >= tmin || (t = b + det) >= tmin) && h.getT() > t) {
                h = Hit(t, static_cast<Object3D *>(this), r.pointAtParameter(t) - center);
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