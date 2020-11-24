#ifndef MY_KD_TREE_H
#define MY_KD_TREE_H

#include "group.hpp"
#include <vector>

class KdTreeNode {
public:
    void initLeaf(const std::vector<int> &new_indicies, std::vector<int> &indicies) {
        _flags = 3;
        _size |= (new_indicies.size() << 2);
        if (new_indicies.size() == 0)
            _index = 0;
        else if (new_indicies.size() == 1)
            _index = new_indicies[0];
        else {
            _offset = indicies.size();
            for (auto i : new_indicies)
                indicies.push_back(i);
        }
    }

    void initInterior(int axis, float split_pos, int right_child) {
        _split = split_pos;
        _flags = axis;
        _rchild |= (right_child << 2);
    }

    bool isLeaf() const { return (_flags & 3) == 3; }
    int rchild() const { return _rchild >> 2; }
    int index() const { return _index; }
    int offset() const { return _offset; }
    int size() const { return _size >> 2; }
    float splitPos() const { return _split; }
    int splitAxis() const { return _flags & 3; }

private:
    union {
        float _split; // for interior
        int _index;   // leaf
        int _offset;  // leaf
    };
    union {
        int _flags;  // both
        int _size;   // leaf
        int _rchild; // interior
    };
};

class KdTree : public Group {
public:
    bool intersect(const Ray &r, Hit &h, float tmin) override;

    void debug() override;

private:
    void add(Object3D *obj) override;

    void build();

    void buildTree(const BoundingBox &bound, const std::vector<int> &indicies, int depth);

    bool intersect_good(const Ray &r, Hit &h, float tmin);

    void debugTree(int id);

    std::vector<Object3D *> bad_objects;
    std::vector<KdTreeNode> nodes;
    std::vector<BoundingBox> bounds;
    std::vector<int> indicies_pool;

    float i_cost, t_cost;
    int max_depth;
    int max_leaf_objects;
    int max_giveup_size;
    BoundingBox world_bound;

    int sum_leaf_size;
    int total_internals;
    int total_leafs;
};

#endif