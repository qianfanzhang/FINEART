#ifndef MY_GROUP_H
#define MY_GROUP_H

#include "hit.hpp"
#include "object/object3d.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

constexpr float TMIN = 1e-4f;

class Group {
public:
    virtual ~Group() = default;

    virtual void add(Object3D *obj) {
        const std::vector<Object3D *> basic_objects = obj->getBasicObjects();
        objects.insert(objects.end(), basic_objects.begin(), basic_objects.end());
    }

    virtual void build() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    bool intersect(const Ray &r, Hit &h) {
        return intersect(r, h, TMIN);
    }

    virtual void debug() {}

protected:
    std::vector<Object3D *> objects;
};

#endif