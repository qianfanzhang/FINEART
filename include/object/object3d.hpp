#ifndef MY_OBJECT3D_H
#define MY_OBJECT3D_H

#include "bounding_box.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "medium.hpp"
#include "ray.hpp"
#include "vecmath.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    explicit Object3D(Material *material, Medium *medium = nullptr)
        : material(material), medium(medium) {}

    virtual ~Object3D() = default;

    virtual Material *getMaterial() const {
        assert(material != nullptr);
        return material;
    }

    virtual Medium *getMedium() const {
        return medium;
    }

    virtual std::vector<Object3D *> getBasicObjects() {
        return {this};
    }

    virtual BoundingBox getBoundingBox() const = 0;

    virtual bool intersect(const Ray &r, Hit &h) = 0;

    virtual Vector2f getUVPoint(const Vector3f &point __attribute__((unused))) const {
        abort();
    }

    virtual std::string getString() const {
        char buff[100];
        snprintf(buff, sizeof(buff), "Object3D()");
        return buff;
    }

private:
    Material *material;
    Medium *medium;
};

#endif
