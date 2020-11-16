#ifndef MY_OBJECT3D_H
#define MY_OBJECT3D_H

#include "hit.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vecmath.h"
#include <cstdlib>

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    explicit Object3D(Material *material)
        : material(material) {}

    virtual ~Object3D() = default;

    virtual Material *getMaterial() const {
        return material;
    }

    // Intersect Ray with this object. If hit, store information in hit structure.
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    virtual const Vector3f &getColor(const Vector3f &point) const {
        if (getMaterial()->getTexture().isConstantColor())
            return getMaterial()->getTexture().getColor();

        const Vector2f &uv_point = getUVPoint(point);
        return getMaterial()->getTexture().getColor(uv_point, point);
    }

    virtual Vector2f getUVPoint(const Vector3f &point) const {
        // not implemented
        abort();
    }

private:
    Material *material;
};

#endif
