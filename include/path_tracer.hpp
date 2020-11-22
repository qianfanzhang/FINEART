#ifndef MY_PATH_TRACER_H
#define MY_PATH_TRACER_H

#include "group/group.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"

class PathTracer {
public:
    PathTracer(Group *group, const Vector3f &background_color, RandomGenerator &gen)
        : group(group),
          background_color(background_color),
          gen(gen) {}

    Vector3f getRadiance(const Ray &ray, int depth = 1);

private:
    Group *group;
    Vector3f background_color;
    RandomGenerator &gen;
};

#endif
