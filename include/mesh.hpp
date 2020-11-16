#ifndef MY_MESH_H
#define MY_MESH_H

#include "object3d.hpp"
#include "triangle.hpp"
#include "vecmath.h"
#include <vector>

class Mesh : public Object3D {
public:
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0;
            x[1] = 0;
            x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    std::vector<Vector3f> v;
    std::vector<TriangleIndex> t;
    std::vector<Vector3f> n;

private:
    // Normal can be used for light estimation
    void computeNormal();
};

#endif
