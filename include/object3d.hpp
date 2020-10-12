#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "hit.hpp"
#include "material.hpp"
#include "ray.hpp"

// Base class for all 3d entities.
class Object3D {
public:
    Material *material;

    Object3D() : material(nullptr) {}
    explicit Object3D(Material *material) : material(material) {}
    virtual ~Object3D() = default;

    // Intersect Ray with this object. If hit, store information in hit structure.
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
};

#endif
