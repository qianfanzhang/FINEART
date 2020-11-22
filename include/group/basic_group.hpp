#ifndef MY_BASIC_GROUP_H
#define MY_BASIC_GROUP_H

#include "group.hpp"

class BasicGroup : public Group {
public:
    BasicGroup() {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool result = false;
        for (auto &obj : objects)
            result |= obj->intersect(r, h, tmin);
        return result;
    }
};

#endif