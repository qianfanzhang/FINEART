#include "hit.hpp"
#include "path_tracing.hpp"
#include "random.hpp"

Vector3f PathTracing::getRadiance(const Ray &ray, int depth) {
    if (depth > max_depth)
        return this->background_color;
    Hit hit;
    bool has_intersection = this->group->intersect(ray, hit, 1e-4);
    if (!has_intersection)
        return this->background_color;

    Vector3f point = ray.pointAtParameter(hit.getT());
    const Vector3f &direction = ray.getDirection();
    const Vector3f &normal = hit.getNormal();
    Material *material = hit.getMaterial();

    if (material->type == DIFFUSE) {
        Vector3f next_d = this->gen.uniformOnHemisphere(normal);
        Ray next_ray(point, next_d);
        return material->emission + material->color * getRadiance(next_ray, depth + 1);

    } else if (material->type == SPECULAR) {
        Vector3f next_d = direction - 2 * Vector3f::dot(normal, direction) * normal;
        Ray next_ray(point, next_d);
        return material->emission + material->color * getRadiance(next_ray, depth + 1);

    } else if (material->type == REFRACTIVE) {
        abort();
    } else
        abort();
}