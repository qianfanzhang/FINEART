#ifndef MY_RAY_H
#define MY_RAY_H

#include "vecmath.h"
#include <iostream>

// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray {
public:
    Ray() {}

    Ray(const Vector3f &orig, const Vector3f &dir) {
        origin = orig;
        direction = dir;
    }

    Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
    }

    Vector3f pointAtParameter(float t) const {
        return origin + direction * t;
    }

    Vector3f origin;
    Vector3f direction;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.origin << ", " << r.direction << ">";
    return os;
}

#endif