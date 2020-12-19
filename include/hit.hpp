#ifndef MY_HIT_H
#define MY_HIT_H

#include "ray.hpp"
#include "utils.hpp"
#include "vecmath.h"

class Object3D;
class Material;

class Hit {
public:
    static constexpr float T_MIN = 1e-4;
    static constexpr float T_MAX = 1e10;

    Hit() {
        t = T_MAX;
        object = nullptr;
    }

    Hit(float t) : t(t), object(nullptr) {}

    Hit(float t, Object3D *object, const Vector2f &uv, const Vector3f &normal)
        : t(t), object(object), uv(uv), normal(normal) {}

    Hit(const Hit &h) {
        t = h.t;
        object = h.object;
        uv = h.uv;
        normal = h.normal;
    }

    ~Hit() = default;

    float getT() const {
        return t;
    }

    void setT(float new_t) {
        t = new_t;
    }

    Object3D *getObject3D() const {
        return object;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    const Vector2f &getUV() const {
        return uv;
    }

    void set(float t, Object3D *object, const Vector2f &uv, const Vector3f &normal) {
        this->t = t;
        this->object = object;
        this->uv = uv;
        this->normal = normal.normalized();
    }

    float t;
    Object3D *object;
    Vector2f uv;
    Vector3f normal;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif
