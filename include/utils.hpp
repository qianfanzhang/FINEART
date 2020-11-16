#ifndef MY_UTILS_H
#define MY_UTILS_H

#include "vecmath.h"
#include <cassert>
#include <cmath>

namespace Utils {

inline float clamp(float x) {
    return x < 0 ? 0 : (x < 1 ? x : 1);
}

// A modf that always returns nonnagative value
inline float fmodp(float x, float y) {
    assert(y > 0);
    double r = std::fmod(x, y);
    return r < 0 ? r + y : r;
}

inline Vector3f getUAxisGivenNormal(const Vector3f &normal) {
    return Vector3f::cross(std::abs(normal.x()) > 0.1 ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0), normal).normalized();
}

inline Vector3f getVAxisGivenNormal(const Vector3f &normal, const Vector3f &u) {
    return Vector3f::cross(normal, u);
}

} // namespace Utils

#endif
