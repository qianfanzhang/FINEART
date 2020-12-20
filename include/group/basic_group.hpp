#ifndef MY_BASIC_GROUP_H
#define MY_BASIC_GROUP_H

#include "group.hpp"
#include <iostream>

class BasicGroup : public Group {
public:
    BasicGroup() {}

    bool intersect(const Ray &r, Hit &h) override {
        bool result = false;
        for (auto &obj : objects) {
            result |= obj->intersect(r, h);
        }
        return result;
    }

    void debug() override {
        std::cout << "[BasicGroup] object=" << objects.size() << std::endl;
    }
};

#endif