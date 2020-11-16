#include "path_tracer.hpp"
#include "hit.hpp"
#include "object3d.hpp"
#include "random.hpp"
#include "vecmath.h"

Vector3f PathTracer::getRadiance(const Ray &ray, int depth) {
    if (depth > MAX_DEPTH)
        return this->background_color;

    Hit hit;
    bool has_intersection = this->group->intersect(ray, hit, 1e-4);
    if (!has_intersection)
        return this->background_color;

    Vector3f point = ray.pointAtParameter(hit.getT());
    Vector3f direction = ray.getDirection();
    Vector3f normal = hit.getNormal();
    Vector3f normal0 = Vector3f::dot(direction, normal) < 0 ? normal : -normal;
    Object3D *object = hit.getObject3D();
    Material *material = object->getMaterial();
    Vector3f emission = material->getEmission();
    Vector3f color = object->getColor(point);

    assert(object != nullptr);
    assert(material != nullptr);

    float max_color = std::max(color.x(), std::max(color.y(), color.z()));
    if (depth >= MIN_DEPTH) {
        if (gen.uniform() < max_color)
            color = color / max_color;
        else
            return emission;
    }

    if (material->getType() == DIFFUSE) {
        Vector3f next_d = this->gen.uniformOnHemisphere(normal);
        Ray next_ray(point, next_d);
        return emission + color * getRadiance(next_ray, depth + 1);

    } else if (material->getType() == SPECULAR) {
        Vector3f next_d = direction - 2 * Vector3f::dot(normal, direction) * normal;
        Ray next_ray(point, next_d);
        return emission + color * getRadiance(next_ray, depth + 1);

    } else if (material->getType() == REFRACTIVE) {
        Vector3f refl_d = direction - 2 * Vector3f::dot(normal, direction) * normal;
        Ray refl_ray(point, refl_d);
        bool into = Vector3f::dot(normal, normal0) > 0;
        float nc = 1, nt = 1.5;
        float nnt = into ? nc / nt : nt / nc;
        float ddn = Vector3f::dot(direction, normal0);
        float cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        // total internal reflection
        if (cos2t < 0)
            return emission + getRadiance(refl_ray, depth + 1);

        // otherwise, choose reflection or refraction
        Vector3f refr_d = (nnt * direction - normal * ((into ? 1 : -1) * (ddn * nnt + std::sqrt(cos2t)))).normalized();
        Ray refr_ray(point, refr_d);
        float a = nt - nc, b = nt + nc;
        float R0 = a * a / (b * b);
        float c = 1 - (into ? -ddn : Vector3f::dot(refr_d, normal));
        float Re = R0 + (1 - R0) * c * c * c * c * c;
        float Tr = 1 - Re;
        if (depth > FULL_REFR_DEPTH) {
            float P = 0.25 + 0.5 * Re;
            float RP = Re / P;
            float TP = Tr / (1 - P);
            if (gen.uniform() < P)
                return emission + getRadiance(refl_ray, depth + 1) * RP;
            else
                return emission + getRadiance(refr_ray, depth + 1) * TP;
        } else {
            Vector3f ret = emission + getRadiance(refl_ray, depth + 1) * Re;
            return ret + getRadiance(refr_ray, depth + 1) * Tr;
        }

    } else
        abort();
}