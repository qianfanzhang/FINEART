#ifndef MY_PATH_TRACER_H
#define MY_PATH_TRACER_H

#include "group/group.hpp"
#include "medium.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"

class PathTracer {
public:
    PathTracer(Group *group, Medium *medium, RandomGenerator &gen)
        : group(group),
          medium(medium),
          gen(gen) {}

    Vector3f getRadiance(Ray ray, int depth = 1);

    static void debug() {
        std::cout << "[PathTracer] MIN_DEPTH=" << MIN_DEPTH << ", MAX_DEPTH=" << MAX_DEPTH << std::endl;
    }

private:
    Group *group;
    Medium *medium;
    RandomGenerator &gen;

    static constexpr int MIN_DEPTH = 3;
    static constexpr int MAX_DEPTH = 6;
    static constexpr float EPS = 1e-4;
};

#endif
