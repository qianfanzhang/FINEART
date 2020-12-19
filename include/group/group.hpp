#ifndef MY_GROUP_H
#define MY_GROUP_H

#include "hit.hpp"
#include "light.hpp"
#include "object/object3d.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

class Group {
public:
    virtual ~Group() = default;

    virtual void add(Object3D *obj) {
        const std::vector<Object3D *> basic_objects = obj->getBasicObjects();
        objects.insert(objects.end(), basic_objects.begin(), basic_objects.end());
    }

    void add(Light *li) {
        lights.push_back(li);
    }

    virtual void build() {}

    bool intersect(const Ray &r, Hit &h) {
        return intersect(r, h, Hit::T_MIN);
    }

    virtual void debug() = 0;

protected:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    std::vector<Object3D *> objects;
    std::vector<Light *> lights;
};

#endif