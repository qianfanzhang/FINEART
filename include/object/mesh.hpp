#ifndef MY_MESH_H
#define MY_MESH_H

#include "object3d.hpp"
#include "vecmath.h"
#include <vector>

class Triangle;

class Mesh : public Object3D {
public:
    Mesh(const char *filename, Material *fallback_material, Matrix4f transform = Matrix4f::identity(), bool use_vec_normal = false);

    std::vector<Object3D *> getBasicObjects() override;

    BoundingBox getBoundingBox() const override {
        return BoundingBox();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    std::string getString() const override {
        char buff[100];
        snprintf(buff, sizeof(buff), "Mesh(%llx,%lu)", (long long)this, triangles.size());
        return buff;
    }

    std::vector<Vector3f> verticies;
    std::vector<Triangle> triangles;
    std::vector<Vector3f> tri_normals;
    std::vector<Vector2f> texcoords;
    std::vector<Vector3f> vec_normals;
    std::vector<Material> materials;
    std::vector<Texture> textures;
    bool use_vec_normal;
};

class Triangle : public Object3D {
public:
    Triangle(Material *material, Mesh *mesh) : Object3D(material), mesh(mesh) {
        _v[0] = _v[1] = _v[2] = 0;
        _t[0] = _t[1] = _t[2] = 0;
        _n[0] = _n[1] = _n[2] = 0;
        id = 0;
    }

    BoundingBox getBoundingBox() const override {
        return BoundingBox(mesh->verticies[_v[0]], mesh->verticies[_v[1]]) + mesh->verticies[_v[2]];
    }

    int &operator[](int i) { return _v[i]; }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    std::string getString() const override {
        char buff[100];
        snprintf(buff, sizeof(buff), "Triangle(%llx,%d,%d,%d)", (long long)mesh, _v[0], _v[1], _v[2]);
        return buff;
    }

    int _v[3];
    int _t[3];
    int _n[3];
    int id; // normal id
    Mesh *mesh;
};

#endif
