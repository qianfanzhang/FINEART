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
    Vector3f ab = mesh->v[_v[1]] - mesh->v[_v[0]];
    Vector3f ac = mesh->v[_v[2]] - mesh->v[_v[0]];
    Vector3f p = Vector3f::cross(r.getDirection(), ac);
    float det = Vector3f::dot(ab, p);
    if (std::abs(det) < EPS)
        return false;

    float invDet = 1 / det;
    Vector3f tv = r.getOrigin() - mesh->v[_v[0]];
    float u = Vector3f::dot(tv, p) * invDet;
    if (u < 0 || u > 1)
        return false;
    Vector3f q = Vector3f::cross(tv, ab);
    float v = Vector3f::dot(r.getDirection(), q) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    float t = Vector3f::dot(ac, q) * invDet;

    if (t >= tmin && t < h.getT()) {
        Vector2f uvcoord = (1 - u - v) * mesh->uv[_uv[0]] + u * mesh->uv[_uv[1]] + v * mesh->uv[_uv[2]];
        Vector3f vec_n = mesh->vec_normal && _n[0] ? (1 - u - v) * mesh->vec_n[_n[0]] + u * mesh->vec_n[_n[1]] + v * mesh->vec_n[_n[2]] : mesh->n[id];
        h.set(t, this, uvcoord, vec_n);
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

Mesh::Mesh(const char *filename, Material *material, Matrix4f transform, bool vec_normal)
    : Object3D(material), vec_normal(vec_normal) {
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
    std::string norTok("vn");
    std::string groupTok("g");
    std::string smoothTok("s");
    char bslash = '/', space = ' ';
    std::string tok;

    v.push_back(Vector3f::ZERO);
    uv.push_back(Vector2f::ZERO);
    n.push_back(Vector3f::ZERO);

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
                std::stringstream ss(line);
                Triangle trig(this);
                ss >> tok;
                for (int ii = 0; ii < 3; ++ii) {
                    ss >> trig._v[ii] >> trig._uv[ii] >> trig._n[ii];
                }
                t.push_back(trig);
                Triangle trig2 = trig;
                ss >> trig2._v[2] >> trig2._uv[2] >> trig2._n[2];
                if (!ss.fail()) {
                    t.push_back(trig2);
                    int tmp;
                    ss >> tmp;
                    assert(ss.fail()); // maximum 4 verticies
                }

            } else {
                Triangle trig(this);
                for (int ii = 0; ii < 3; ++ii) {
                    ss >> trig[ii];
                }
                t.push_back(trig);
                Triangle trig2 = trig;
                ss >> trig2._v[2];
                if (!ss.fail()) {
                    t.push_back(trig2);
                    int tmp;
                    ss >> tmp;
                    assert(ss.fail()); // maximum 4 verticies
                }
            }
        } else if (tok == texTok) {
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
            uv.push_back(texcoord);
        } else if (tok == norTok) {
            Vector3f norcoord;
            ss >> norcoord[0];
            ss >> norcoord[1];
            ss >> norcoord[2];
            vec_n.push_back(Utils::transformNormal(transform, norcoord));
        } else if (tok == groupTok) {

        } else if (tok == smoothTok) {

        } else {
            std::cerr << "unrecognized line" << std::endl;
            std::cout << "\t" << line << std::endl;
            abort();
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
