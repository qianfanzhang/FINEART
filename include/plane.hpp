#ifndef MY_PLANE_H
#define MY_PLANE_H

#include "object3d.hpp"
#include "vecmath.h"
#include <cmath>

class Plane : public Object3D {
public:
    Plane() = delete;

    Plane(const Vector3f &normal, float d, Material *m)
        : Object3D(m), normal(normal), d(d) {
        this->normal.normalize();
        u_axis = Utils::getUAxisGivenNormal(this->normal);
        v_axis = Utils::getVAxisGivenNormal(this->normal, u_axis);
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float v = Vector3f::dot(normal, r.getDirection());
        if (v != 0) {
            float t = (d - Vector3f::dot(normal, r.getOrigin())) / v;
            if (t >= tmin && h.getT() > t) {
                h = Hit(t, static_cast<Object3D *>(this), v < 0 ? normal : -normal);
                return true;
            }
        }
        return false;
    }

    Vector2f getUVPoint(const Vector3f &point) const {
        float u = Utils::fmodp(Vector3f::dot(point, u_axis), 1);
        float v = Utils::fmodp(Vector3f::dot(point, v_axis), 1);
        return Vector2f(u, v);
    }

protected:
    // equation: normal.x = d
    Vector3f normal;
    float d;

    Vector3f u_axis, v_axis;
};

#endif