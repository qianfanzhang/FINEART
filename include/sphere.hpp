#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <cmath>
#include <cstdlib>
#include <vecmath.h>

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
            if (((t = b - det) >= tmin || (t = b + det) >= tmin) && h.getT() > t) {
                h = Hit(t, material, r.pointAtParameter(t) - center);
                return true;
            }
        }
        return false;
    }

protected:
    Vector3f center;
    float radius;
};

#endif