#ifndef MY_GROUP_H
#define MY_GROUP_H

#include "hit.hpp"
#include "light.hpp"
#include "medium.hpp"
#include "material.hpp"
#include "object/object3d.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"
#include <iostream>
#include <vector>

class Group {
public:
    Group() {
        world_bound = nullptr;
    }

    virtual ~Group() = default;

    void add(Light *li) {
        lights.push_back(li);
    }

    virtual void add(Object3D *obj) {
        const std::vector<Object3D *> basic_objects = obj->getBasicObjects();
        objects.insert(objects.end(), basic_objects.begin(), basic_objects.end());
    }

    Vector3f sampleAllLights(const Ray &ray, const Hit &hit, Material *material, Medium *medium, RandomGenerator &gen) {
        assert(material != nullptr || medium != nullptr);

        BSDF *bsdf = material ? material->sampleBSDF(gen) : nullptr;
        float beta = 1;
        if (material != nullptr)
            assert(bsdf != nullptr);
        if (bsdf != nullptr && bsdf->isDelta())
            return Vector3f::ZERO;

        Vector3f L = Vector3f::ZERO;
        Vector3f point = ray.pointAtParameter(hit.t);

        for (Light *li : lights) {
            Vector3f dir;
            float li_t;
            Vector3f intensity = li->sampleRay(point, dir, li_t, gen);
            if (intensity.length() < 1e-3)
                continue;
            Hit tmp_hit;
            tmp_hit.t = li_t;
            bool intersected = intersect(Ray(point, dir), tmp_hit);
            if (!intersected || tmp_hit.object == world_bound) {
                Vector3f tmp_beta = 1;
                if (medium != nullptr) {
                    Ray medium_ray = ray;
                    bool has_medium_interaction = medium->interact(medium_ray, tmp_beta, gen, tmp_hit.t);
                    if (has_medium_interaction)
                        continue;
                }
                if (bsdf != nullptr) {
                    float f = bsdf->pdf(dir, -ray.direction, hit.normal) * std::abs(Vector3f::dot(dir, hit.normal));
                    L += tmp_beta * beta * intensity * f;
                } else {
                    float f = medium->pdf(dir, -ray.direction);
                    L += tmp_beta * beta * intensity * f;
                }
            }
        }

        return L;
    }

    virtual bool intersect(const Ray &r, Hit &h) = 0;

    virtual void build() {}

    virtual void debug() = 0;

    std::vector<Light *> lights;

    Object3D *world_bound;

protected:
    std::vector<Object3D *> objects;
};

#endif