#ifndef MY_SPHERE_H
#define MY_SPHERE_H

#include "bounding_box.hpp"
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
                h.set(t, this, r.pointAtParameter(t) - center);
                return true;
            }
        }
        return false;
    }

    Vector2f getUVPoint(const Vector3f &point) const {
        Vector3f d = (point - center).normalized();
        float phi = std::atan2(d.y(), d.x());
        float theta = std::asin(d.z());
        float u = 0.5 + phi / (2 * Utils::pi);
        float v = 0.5 + theta / Utils::pi;
        return Vector2f(u, v);
    }

    BoundingBox getBoundingBox() const override {
        return BoundingBox(center - Vector3f(radius, radius, radius),
                           center + Vector3f(radius, radius, radius));
    }

    std::string getString() const override {
        char buff[100];
        snprintf(buff, sizeof(buff), "Sphere((%.2f,%.2f,%.2f),%.2f)", center.x(), center.y(), center.z(), radius);
        return buff;
    }

private:
    Vector3f center;
    float radius;
};

#endif