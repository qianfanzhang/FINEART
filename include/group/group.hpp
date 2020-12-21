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

        BSDF *bsdf = nullptr;
        float beta = material == nullptr ? 1 : material->sampleBSDF(bsdf, gen);
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
            if (!intersect(Ray(point, dir), tmp_hit)) {
                // FIXME: unhandleled medium interaction
                if (bsdf != nullptr) {
                    float f = bsdf->pdf(dir, -ray.direction, hit.normal) * std::abs(Vector3f::dot(dir, hit.normal));
                    L += beta * intensity * f;
                } else {
                    float f = medium->pdf(dir, -ray.direction);
                    L += beta * intensity * f;
                }
            }
        }

        return L;
    }

    virtual bool intersect(const Ray &r, Hit &h) = 0;

    virtual void build() {}

    virtual void debug() = 0;

    std::vector<Light *> lights;

protected:
    std::vector<Object3D *> objects;
};

#endif