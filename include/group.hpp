#ifndef MY_GROUP_H
#define MY_GROUP_H

#include "hit.hpp"
#include "object3d.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {
public:
    Group() {}

    ~Group() override {
        for (auto obj : objects)
            delete obj;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool result = false;
        for (auto &obj : objects)
            result |= obj->intersect(r, h, tmin);
        return result;
    }

    void addObject(Object3D *obj) {
        objects.push_back(obj);
    }

    int getGroupSize() const {
        return objects.size();
    }

protected:
    std::vector<Object3D *> objects;
};

#endif