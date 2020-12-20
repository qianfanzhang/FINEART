#define TINYOBJLOADER_IMPLEMENTATION

#include "object/mesh.hpp"
#include "tiny_obj_loader.hpp"
#include "utils.hpp"
#include "vecmath.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

static const float EPS = 1e-6;

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    Vector3f ab = mesh->verticies[_v[1]] - mesh->verticies[_v[0]];
    Vector3f ac = mesh->verticies[_v[2]] - mesh->verticies[_v[0]];
    Vector3f p = Vector3f::cross(r.getDirection(), ac);
    float det = Vector3f::dot(ab, p);
    if (std::abs(det) < EPS)
        return false;

    float invDet = 1 / det;
    Vector3f tv = r.getOrigin() - mesh->verticies[_v[0]];
    float u = Vector3f::dot(tv, p) * invDet;
    if (u < 0 || u > 1)
        return false;
    Vector3f q = Vector3f::cross(tv, ab);
    float v = Vector3f::dot(r.getDirection(), q) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    float t = Vector3f::dot(ac, q) * invDet;

    if (t >= tmin && t < h.getT()) {
        Vector2f texcoord = (1 - u - v) * mesh->texcoords[_t[0]] + u * mesh->texcoords[_t[1]] + v * mesh->texcoords[_t[2]];
        Vector3f normal = (1 - u - v) * mesh->vec_normals[_n[0]] + u * mesh->vec_normals[_n[1]] + v * mesh->vec_normals[_n[2]] + mesh->tri_normals[id];
        // printf("%.2f %.2f %.2f\n", normal.x(), normal.y(), normal.z());
        h.set(t, this, texcoord, normal);
        return true;
    }
    return false;
}

std::vector<Object3D *> Mesh::getBasicObjects() {
    std::vector<Object3D *> objs;
    for (auto &triangle : triangles)
        objs.push_back(&triangle);
    return objs;
}

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) {
    bool result = false;
    for (auto &triangle : triangles)
        result |= triangle.intersect(r, h, tmin);
    return result;
}

Mesh::Mesh(const char *filename, Material *fallback_material, Matrix4f transform, bool use_vec_normal)
    : Object3D(fallback_material), use_vec_normal(use_vec_normal) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> obj_materials;

    std::string err_msg;
    bool load_success = tinyobj::LoadObj(&attrib, &shapes, &obj_materials, &err_msg, filename);

    if (!err_msg.empty())
        printf("[Mesh] error message: %s\n", err_msg.c_str());
    if (!load_success) {
        printf("[Mesh] failed loading obj file %s, aborting\n", filename);
        fflush(stdout);
        abort();
    }

    assert(attrib.vertices.size() % 3 == 0);
    verticies.resize(attrib.vertices.size() / 3 + 1);
    for (int v = 0; v < verticies.size() - 1; ++v) {
        verticies[v + 1][0] = attrib.vertices[3 * v + 0];
        verticies[v + 1][1] = attrib.vertices[3 * v + 1];
        verticies[v + 1][2] = attrib.vertices[3 * v + 2];
        verticies[v + 1] = Utils::transformPoint(transform, verticies[v + 1]);
    }

    assert(attrib.normals.size() % 3 == 0);
    vec_normals.resize(attrib.normals.size() / 3 + 1);
    for (int n = 0; n < vec_normals.size() - 1; ++n) {
        vec_normals[n + 1][0] = attrib.normals[3 * n + 0];
        vec_normals[n + 1][1] = attrib.normals[3 * n + 1];
        vec_normals[n + 1][2] = attrib.normals[3 * n + 2];
        vec_normals[n + 1] = Utils::transformNormal(transform, vec_normals[n + 1]);
    }

    assert(attrib.texcoords.size() % 2 == 0);
    texcoords.resize(attrib.texcoords.size() / 2 + 1);
    for (int t = 0; t < texcoords.size() - 1; ++t) {
        texcoords[t + 1][0] = attrib.texcoords[2 * t + 0];
        texcoords[t + 1][1] = attrib.texcoords[2 * t + 1];
    }

    materials.reserve(obj_materials.size());
    textures.reserve(obj_materials.size());
    for (auto &m : obj_materials) {
        Material material;
        material.diffuse = {m.diffuse[0], m.diffuse[1], m.diffuse[2]};

        if (!m.diffuse_texname.empty()) {
            textures.emplace_back("texture/" + m.diffuse_texname, Vector3f(m.diffuse_texopt.scale));
            material.texture = &textures.back();
        }
        material.type = DIFFUSE;
        // if (m.dissolve == 1)
        //     material.type = DIFFUSE;
        // else
        //     material.type = REFRACTIVE;
        material.emission = Vector3f(m.emission);

        materials.push_back(material);
    }

    for (auto &shape : shapes) {
        int index_offset = 0;
        for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f];
            int mid = shape.mesh.material_ids[f];
            assert(fv == 3);

            Triangle triangle(mid == -1 ? fallback_material : &materials[mid], this);
            for (int i = 0; i < fv; ++i) {
                triangle._v[i] = shape.mesh.indices[index_offset + i].vertex_index + 1;
                triangle._n[i] = use_vec_normal ? shape.mesh.indices[index_offset + i].normal_index + 1 : 0;
                triangle._t[i] = shape.mesh.indices[index_offset + i].texcoord_index + 1;
                assert(triangle._v[i] >= 0 && triangle._v[i] < verticies.size());
                assert(triangle._n[i] >= 0 && triangle._n[i] < vec_normals.size());
                assert(triangle._t[i] >= 0 && triangle._t[i] < texcoords.size());
            }
            triangles.push_back(triangle);

            index_offset += fv;
        }
    }

    if (!use_vec_normal) {
        tri_normals.resize(triangles.size());
        for (int i = 0; i < triangles.size(); ++i) {
            Triangle &triangle = triangles[i];
            Vector3f a = verticies[triangle[1]] - verticies[triangle[0]];
            Vector3f b = verticies[triangle[2]] - verticies[triangle[0]];
            tri_normals[i] = Vector3f::cross(a, b).normalized();
            triangle.id = i;
        }
    } else {
        tri_normals.push_back(Vector3f::ZERO);
        for (auto &triangle : triangles)
            triangle.id = 0;
    }
}