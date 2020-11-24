#ifndef MY_MESH_H
#define MY_MESH_H

#include "object3d.hpp"
#include "vecmath.h"
#include <vector>

class Triangle;

class Mesh : public Object3D {
public:
    Mesh(const char *filename, Material *m, Matrix4f transform = Matrix4f::identity(), bool vec_normal = false);

    std::vector<Object3D *> getBasicObjects() override;

    BoundingBox getBoundingBox() const override {
        return BoundingBox();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    std::string getString() const override {
        char buff[100];
        snprintf(buff, sizeof(buff), "Mesh(%llx,%lu)", (long long)this, t.size());
        return buff;
    }

    std::vector<Vector3f> v;
    std::vector<Triangle> t;
    std::vector<Vector3f> n;
    std::vector<Vector2f> uv;
    std::vector<Vector3f> vec_n;
    bool vec_normal;
};

class Triangle : public Object3D {
public:
    Triangle(Mesh *mesh) : mesh(mesh) {
        _v[0] = _v[1] = _v[2] = 0;
        _uv[0] = _uv[1] = _uv[2] = 0;
        _n[0] = _n[1] = _n[2] = 0;
        id = 0;
    }

    Material *getMaterial() const override {
        return mesh->getMaterial();
    }

    BoundingBox getBoundingBox() const override {
        return BoundingBox(mesh->v[_v[0]], mesh->v[_v[1]]) + mesh->v[_v[2]];
    }

    int &operator[](int i) { return _v[i]; }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    std::string getString() const override {
        char buff[100];
        snprintf(buff, sizeof(buff), "Triangle(%llx,%d,%d,%d)", (long long)mesh, _v[0], _v[1], _v[2]);
        return buff;
    }

    int _v[3];
    int _uv[3];
    int _n[3];
    int id; // normal id
    Mesh *mesh;
};

#endif
