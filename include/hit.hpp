#ifndef MY_HIT_H
#define MY_HIT_H

#include "ray.hpp"
#include "vecmath.h"

class Object3D;
class Material;

class Hit {
public:
    // constructors
    Hit() {
        t = 1e38;
        object = nullptr;
    }

    Hit(float t, Object3D *object, const Vector3f &normal) {
        this->t = t;
        this->object = object;
        this->normal = normal.normalized();
    }

    Hit(const Hit &h) {
        t = h.t;
        object = h.object;
        normal = h.normal;
    }

    // destructor
    ~Hit() = default;

    float getT() const {
        return t;
    }

    Object3D *getObject3D() const {
        return object;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    void set(float t, Object3D *object, const Vector3f &normal) {
        this->t = t;
        this->object = object;
        this->normal = normal;
    }

private:
    float t;
    Object3D *object;
    Vector3f normal;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif
