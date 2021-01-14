#ifndef MY_PATH_TRACER_H
#define MY_PATH_TRACER_H

#include "group/group.hpp"
#include "light.hpp"
#include "medium.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"

class PathTracer {
public:
    PathTracer(Group *group, Medium *medium, SkyLight *sky_light, RandomGenerator &gen)
        : group(group),
          medium(medium),
          sky_light(sky_light),
          gen(gen) {}

    Vector3f getRadiance(Ray ray, Medium *start_medium) {
        return getRadiance(ray, 1, start_medium == nullptr ? medium : start_medium);
    }

    Vector3f getRadiance(Ray ray, int depth, Medium *medium);

    static void debug() {
        std::cout << "[PathTracer] MIN_DEPTH=" << MIN_DEPTH << ", MAX_DEPTH=" << MAX_DEPTH << std::endl;
    }

private:
    Group *group;
    Medium *medium;
    SkyLight *sky_light;
    RandomGenerator &gen;

    static constexpr int MIN_DEPTH = 4;
    static constexpr int MAX_DEPTH = 9;
    static constexpr float EPS = 1e-4;
};

#endif
