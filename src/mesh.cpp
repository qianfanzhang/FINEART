#include "object/mesh.hpp"
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
    Vector3f ab = mesh->v[v[1]] - mesh->v[v[0]];
    Vector3f ac = mesh->v[v[2]] - mesh->v[v[0]];
    Vector3f p = Vector3f::cross(r.getDirection(), ac);
    float det = Vector3f::dot(ab, p);
    if (std::abs(det) < EPS)
        return -1;

    float invDet = 1 / det;
    Vector3f tv = r.getOrigin() - mesh->v[v[0]];
    float u = Vector3f::dot(tv, p) * invDet;
    if (u < 0 || u > 1)
        return -1;
    Vector3f q = Vector3f::cross(tv, ab);
    float v = Vector3f::dot(r.getDirection(), q) * invDet;
    if (v < 0 || u + v > 1)
        return -1;

    float t = Vector3f::dot(ac, q) * invDet;
    if (t >= tmin && t < h.getT()) {
        h.set(t, this, mesh->n[id]);
        return true;
    }
    return false;
}

std::vector<Object3D *> Mesh::getBasicObjects() {
    std::vector<Object3D *> objs;
    for (auto &triangle : t)
        objs.push_back(&triangle);
    return objs;
}

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) {
    bool result = false;
    for (int triId = 0; triId < t.size(); ++triId) {
        Triangle &triangle = t[triId];
        result |= triangle.intersect(r, h, tmin);
    }
    return result;
}

Mesh::Mesh(const char *filename, Material *material, Matrix4f transform) : Object3D(material) {
    std::ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        std::cout << "Cannot open " << filename << "\n";
        return;
    }
    std::string line;
    std::string vTok("v");
    std::string fTok("f");
    std::string texTok("vt");
    char bslash = '/', space = ' ';
    std::string tok;
    int texID;
    while (true) {
        std::getline(f, line);
        if (f.eof()) {
            break;
        }
        if (line.size() < 3) {
            continue;
        }
        if (line.at(0) == '#') {
            continue;
        }
        std::stringstream ss(line);
        ss >> tok;
        if (tok == vTok) {
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            v.push_back(Utils::transformPoint(transform, vec));
        } else if (tok == fTok) {
            if (line.find(bslash) != std::string::npos) {
                std::replace(line.begin(), line.end(), bslash, space);
                std::stringstream facess(line);
                Triangle trig(this);
                facess >> tok;
                for (int ii = 0; ii < 3; ii++) {
                    facess >> trig[ii] >> texID;
                    trig[ii]--;
                }
                t.push_back(trig);
            } else {
                Triangle trig(this);
                for (int ii = 0; ii < 3; ii++) {
                    ss >> trig[ii];
                    trig[ii]--;
                }
                t.push_back(trig);
            }
        } else if (tok == texTok) {
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
        }
    }
    f.close();

    n.resize(t.size());
    for (int triId = 0; triId < (int)t.size(); ++triId) {
        Triangle &triangle = t[triId];
        Vector3f a = v[triangle[1]] - v[triangle[0]];
        Vector3f b = v[triangle[2]] - v[triangle[0]];
        n[triId] = Vector3f::cross(a, b).normalized();
        triangle.id = triId;
    }
}
