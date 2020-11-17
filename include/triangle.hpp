#ifndef MY_TRIANGLE_H
#define MY_TRIANGLE_H

#include "object3d.hpp"
#include "vecmath.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

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

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        const float EPS = 1e-4;
        Vector3f ab = b - a;
        Vector3f ac = c - a;
        Vector3f p = Vector3f::cross(r.getDirection(), ac);
        float det = Vector3f::dot(ab, p);
        if (det < EPS)
            return false;

        float invDet = 1 / det;
        Vector3f tv = r.getOrigin() - a;
        float u = Vector3f::dot(tv, p) * invDet;
        if (u < 0 || u > 1)
            return false;
        Vector3f q = Vector3f::cross(tv, ab);
        float v = Vector3f::dot(r.getDirection(), q) * invDet;
        if (v < 0 || u + v > 1)
            return false;

        float t = Vector3f::dot(ac, q) * invDet;
        if (t >= tmin && t < h.getT()) {
            h = Hit(t, static_cast<Object3D *>(this), normal);
            return true;
        }

        return false;
    }

protected:
    Vector3f a, b, c;
    Vector3f normal;
};

#endif