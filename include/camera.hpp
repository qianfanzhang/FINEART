#ifndef MY_CAMERA_H
#define MY_CAMERA_H

#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"
#include <cmath>
#include <float.h>

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction,
           const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up.normalized());
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point, RandomGenerator &gen) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
                      const Vector3f &up, float angle, int imgW, int imgH)
        : Camera(center, direction, up, imgW, imgH) {
        float t = 2 * tanf(angle / 2);
        offset = Vector2f(this->width / 2.f, this->height / 2.f);
        focal_length = Vector2f(this->width / t, this->width / t);
        rotation = Matrix3f(this->horizontal, this->up, this->direction);
    }

    Ray generateRay(const Vector2f &point, RandomGenerator &gen) override {
        Vector3f d((point - offset) / focal_length, 1);
        return Ray(center, (rotation * d).normalized());
    }

protected:
    Vector2f offset;
    Vector2f focal_length;
    Matrix3f rotation;
};

class ThinLenCamera : public PerspectiveCamera {
public:
    ThinLenCamera(const Vector3f &center, const Vector3f &direction,
                  const Vector3f &up, float angle, float aperture, float focus_dist, int imgW, int imgH)
        : PerspectiveCamera(center, direction, up, angle, imgW, imgH) {
        this->len_radius = aperture / 2;
        this->focus_dist = focus_dist;
    }

    Ray generateRay(const Vector2f &point, RandomGenerator &gen) override {
        Vector3f blur = rotation * Vector3f(len_radius * gen.uniformInDisk(), 0);
        Vector3f d((point - offset) / focal_length, 1);
        return Ray(center + blur, (focus_dist * (rotation * d) - blur).normalized());
    }

protected:
    float len_radius;
    float focus_dist;
};

#endif