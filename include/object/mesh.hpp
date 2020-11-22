#ifndef MY_MESH_H
#define MY_MESH_H

#include "object3d.hpp"
#include "vecmath.h"
#include <vector>

class Triangle;

class Mesh : public Object3D {
public:
    Mesh(const char *filename, Material *m, Matrix4f transform = Matrix4f::identity());

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
};

class Triangle : public Object3D {
public:
    Triangle(Mesh *mesh) : mesh(mesh) {
        v[0] = v[1] = v[2] = 0;
        id = 0;
    }

    Material *getMaterial() const override {
        return mesh->getMaterial();
    }

    BoundingBox getBoundingBox() const override {
        return BoundingBox(mesh->v[v[0]], mesh->v[v[1]]) + mesh->v[v[2]];
    }

    int &operator[](int i) { return v[i]; }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    std::string getString() const override {
        char buff[100];
        snprintf(buff, sizeof(buff), "Triangle(%llx,%d,%d,%d)", (long long)mesh, v[0], v[1], v[2]);
        return buff;
    }

    int id;

private:
    int v[3];
    Mesh *mesh;
};

#endif
