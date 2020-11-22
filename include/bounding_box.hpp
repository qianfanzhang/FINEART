#ifndef MY_BOUNDING_BOX_H
#define MY_BOUNDING_BOX_H

#include "ray.hpp"
#include "utils.hpp"
#include "vecmath.h"
#include <limits>

static const float MIN = -1e10f;
static const float MAX = 1e10f;

struct BoundingBox {
    BoundingBox()
        : min(MIN, MIN, MIN), max(MAX, MAX, MAX) {}
    BoundingBox(const Vector3f &p1, const Vector3f &p2)
        : min(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()), std::min(p1.z(), p2.z())),
          max(std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()), std::max(p1.z(), p2.z())) {}

    friend BoundingBox operator+(const BoundingBox &a, const Vector3f &b) {
        return BoundingBox(Vector3f(std::min(a.min.x(), b.x()), std::min(a.min.y(), b.y()), std::min(a.min.z(), b.z())),
                           Vector3f(std::max(a.max.x(), b.x()), std::max(a.max.y(), b.y()), std::max(a.max.z(), b.z())));
    }

    friend BoundingBox operator+(const BoundingBox &a, const BoundingBox &b) {
        return BoundingBox(Vector3f(std::min(a.min.x(), b.min.x()), std::min(a.min.y(), b.min.y()), std::min(a.min.z(), b.min.z())),
                           Vector3f(std::max(a.max.x(), b.max.x()), std::max(a.max.y(), b.max.y()), std::max(a.max.z(), b.max.z())));
    }

    bool infinite() const {
        return max == Vector3f(MAX, MAX, MAX) && min == Vector3f(MIN, MIN, MIN);
    }

    Vector3f diagonal() const {
        return max - min;
    }

    float surfaceArea() const {
        Vector3f d = diagonal();
        return 2 * (d.x() * d.y() + d.x() * d.z() + d.y() * d.z());
    }

    int maxAxis() const {
        Vector3f d = diagonal();
        if (d.x() > d.y() && d.x() > d.z())
            return 0;
        if (d.y() > d.z())
            return 1;
        return 2;
    }

    bool intersect(const Ray &ray, float &tmin, float &tmax) {
        for (int i = 0; i < 3; ++i) {
            float inv_dir = 1 / ray.getDirection()[i];
            float t_near = (min[i] - ray.getOrigin()[i]) * inv_dir;
            float t_far = (max[i] - ray.getOrigin()[i]) * inv_dir;
            if (t_near > t_far)
                std::swap(t_near, t_far);
            tmin = t_near > tmin ? t_near : tmin;
            tmax = t_far < tmax ? t_far : tmax;
            // tmax *= 1 + 2 * Utils::gamma(3); // for precision, see pbrt
            if (tmin > tmax)
                return false;
        }
        return true;
    }

    void debug() const {
        printf("BoundingBox((%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f))",
               min.x(), min.y(), min.z(), max.x(), max.y(), max.z());
    }

    Vector3f min;
    Vector3f max;
};

#endif