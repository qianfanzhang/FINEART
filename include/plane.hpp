#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <cmath>
#include <vecmath.h>

class Plane : public Object3D {
public:
    Plane() = delete;
    Plane(const Vector3f &normal, float d, Material *m)
        : Object3D(m), normal(normal), d(d) {}
    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float v = Vector3f::dot(normal, r.getDirection());
        if (v != 0) {
            float t = (d - Vector3f::dot(normal, r.getOrigin())) / v;
            if (t >= tmin && h.getT() > t) {
                h = Hit(t, material, normal);
                return true;
            }
        }
        return false;
    }

protected:
    Vector3f normal;
    float d;
};

#endif