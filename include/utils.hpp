#ifndef MY_UTILS_H
#define MY_UTILS_H

#include "vecmath.h"
#include <cassert>
#include <cmath>
#include <limits>

namespace Utils {

constexpr float pi = 3.1415926536f;
constexpr float inv4pi = 1 / (4 * pi);
constexpr float MACHINE_EPS = std::numeric_limits<float>::epsilon() * 0.5;
constexpr float MAX_FLOAT = std::numeric_limits<float>::max();
constexpr float MIN_FLOAT = std::numeric_limits<float>::min();

inline constexpr float gamma(int n) {
    return (n * MACHINE_EPS) / (1 - n * MACHINE_EPS);
}

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

inline Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
    return (mat * Vector4f(point, 1)).xyz();
}

inline Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
    return (mat * Vector4f(dir, 0)).xyz();
}

inline Vector3f transformNormal(const Matrix4f &mat, const Vector3f &dir) {
    return (mat.inverse().transposed() * Vector4f(dir, 0)).xyz();
}

} // namespace Utils

#endif
