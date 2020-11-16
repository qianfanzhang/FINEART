#ifndef MY_PATH_TRACING_H
#define MY_PATH_TRACING_H

#include "group.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"

class PathTracing {
public:
    PathTracing(Group *group, const Vector3f &background_color, int max_depth, RandomGenerator &gen)
        : group(group),
          background_color(background_color),
          max_depth(max_depth),
          gen(gen) {}

    Vector3f getRadiance(const Ray &ray, int depth = 1);

private:
    Group *group;
    Vector3f background_color;
    int max_depth;
    RandomGenerator &gen;
};

#endif
