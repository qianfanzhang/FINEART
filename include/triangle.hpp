#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <cmath>
#include <iostream>
#include <vecmath.h>

class Triangle : public Object3D {
public:
    Triangle() = delete;
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m)
        : Object3D(m), a(a), b(b), c(c) {
        normal = Vector3f::cross(b - a, c - a).normalized();
    }
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m, const Vector3f &n)
        : Object3D(m), a(a), b(b), c(c), normal(n) {
        normal = Vector3f::cross(b - a, c - a).normalized();
    }

    bool inside2d(const Vector3f &p) {
        Vector3f pa = a - p;
        Vector3f pb = b - p;
        Vector3f pc = c - p;
        return Vector3f::dot(Vector3f::cross(pa, pb), normal) >= 0 && Vector3f::dot(Vector3f::cross(pb, pc), normal) >= 0 && Vector3f::dot(Vector3f::cross(pc, pa), normal) >= 0;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float v = Vector3f::dot(normal, r.getDirection());
        float d = Vector3f::dot(normal, a);
        if (v != 0) {
            float t = (d - Vector3f::dot(normal, r.getOrigin())) / v;
            if (t >= tmin && h.getT() > t && inside2d(r.pointAtParameter(t))) {
                h = Hit(t, material, normal);
                return true;
            }
        }
        return false;
    }

protected:
    Vector3f a, b, c;
    Vector3f normal;
};

#endif