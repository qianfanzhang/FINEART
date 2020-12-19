#ifndef MY_PATH_TRACER_H
#define MY_PATH_TRACER_H

#include "group/group.hpp"
#include "medium.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"

class PathTracer {
public:
    PathTracer(Group *group, Medium *medium, const Vector3f &background_color, RandomGenerator &gen)
        : group(group),
          medium(medium),
          background_color(background_color),
          gen(gen) {}

    Vector3f getRadiance(Ray ray, int depth = 1);

    static void debug() {
        std::cout << "[PathTracer] MIN_DEPTH=" << MIN_DEPTH << ", MAX_DEPTH=" << MAX_DEPTH << ", FULL_REFR_DEPTH=" << FULL_REFR_DEPTH << std::endl;
    }

private:
    Group *group;
    Medium *medium;
    Vector3f background_color;
    RandomGenerator &gen;

    static const int MIN_DEPTH = 5;
    static const int MAX_DEPTH = 11;
    static const int FULL_REFR_DEPTH = 2;
};

#endif
