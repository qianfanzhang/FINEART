#include "group/kd_tree.hpp"
#include <cmath>
#include <limits>
#include <stack>
#include <utility>

void KdTree::add(Object3D *obj) {
    const std::vector<Object3D *> basic_objects = obj->getBasicObjects();
    for (auto obj : basic_objects) {
        BoundingBox bound = obj->getBoundingBox();
        if (bound.infinite())
            bad_objects.push_back(obj);
        else {
            world_bound = objects.empty() ? bound : world_bound + bound;
            objects.push_back(obj);
            bounds.push_back(bound);
        }
    }
}

void KdTree::build() {
    // max_depth = -1;
    max_depth = std::round(7 + std::log2(objects.size()));
    max_leaf_objects = 3;
    max_giveup_size = 16;
    i_cost = 80;
    t_cost = 1;

    std::vector<int> init_indicies;
    for (int i = 0; i < objects.size(); ++i) {
        init_indicies.push_back(i);
    }
    buildTree(world_bound, init_indicies, 0);
}

void KdTree::buildTree(const BoundingBox &bound, const std::vector<int> &indicies, int depth) {
    int id = nodes.size();
    // printf("buildTree(id=%d, bound=", id);
    // bound.debug();
    // printf(", size=%ld, depth=%d\n", indicies.size(), depth);
    // for (auto i : indicies) {
    //     printf("%d ", i);
    // }
    // printf("\n");

    nodes.push_back(KdTreeNode());
    if (indicies.size() <= max_leaf_objects || depth >= max_depth) {
        nodes[id].initLeaf(indicies, indicies_pool);
        return;
    }

    int best_axis = -1, best_offset = -1;
    float best_cost = std::numeric_limits<float>::max();
    float old_cost = i_cost * indicies.size();
    float total_area = bound.surfaceArea();
    float inv_total_area = 1 / total_area;
    Vector3f d = bound.diagonal();
    std::vector<std::pair<float, int>> lines[3];

    for (int axis = 0; axis < 3; ++axis) {
        lines[axis].clear();

        for (int i : indicies) {
            lines[axis].push_back({bounds[i].min[axis], i << 1 | 1});
            lines[axis].push_back({bounds[i].max[axis], i << 1});
        }
        std::sort(lines[axis].begin(), lines[axis].end());

        int n_below = 0;
        int n_above = indicies.size();
        float d1 = d[(axis + 1) % 3], d2 = d[(axis + 2) % 3];

        for (int i = 0; i < lines[axis].size(); ++i) {
            if ((lines[axis][i].second & 1) == 0)
                --n_above;

            float t = lines[axis][i].first;
            // printf("t=%.4f, n_below=%d, n_above=%d\n", t, n_below, n_above);

            if (t >= bound.min[axis] && t <= bound.max[axis]) {
                float below_area = 2 * (d1 * d2 + (t - bound.min[axis]) * (d1 + d2));
                float above_area = 2 * (d1 * d2 + (bound.max[axis] - t) * (d1 + d2));
                float p_below = below_area * inv_total_area;
                float p_above = above_area * inv_total_area;
                // printf("%d %d %.3f %.3f\n", n_below, n_above, p_below, p_above);
                float cost = t_cost + i_cost * (p_below * n_below + p_above * n_above);
                if (cost < best_cost) {
                    best_cost = cost;
                    best_axis = axis;
                    best_offset = i;
                }
            }

            if ((lines[axis][i].second & 1) == 1)
                ++n_below;
        }
        assert(n_below == indicies.size());
    }

    if (best_axis == -1 || (best_cost > 4 * old_cost && indicies.size() < max_giveup_size)) {
        nodes[id].initLeaf(indicies, indicies_pool);
        return;
    }

    std::vector<int> new_indicies;
    float t = lines[best_axis][best_offset].first;
    BoundingBox bound0 = bound, bound1 = bound;
    bound0.max[best_axis] = bound1.min[best_axis] = t;

    // printf("best_axis=%d, t=%.3f, best_cost=%.3f, old_cost=%.3f\n", best_axis, t, best_cost, old_cost);

    for (int i = 0; i < best_offset; ++i) {
        if ((lines[best_axis][i].second & 1) == 1)
            new_indicies.push_back(lines[best_axis][i].second >> 1);
    }
    // printf(">%d lc, ", id);
    buildTree(bound0, new_indicies, depth + 1);

    nodes[id].initInterior(best_axis, t, nodes.size());

    new_indicies.clear();
    for (int i = best_offset; i < lines[best_axis].size(); ++i) {
        if ((lines[best_axis][i].second & 1) == 0)
            new_indicies.push_back(lines[best_axis][i].second >> 1);
    }
    // printf(">%d rc, ", id);
    buildTree(bound1, new_indicies, depth + 1);
}

struct Candidate {
    int id;
    float tmin, tmax;
};

bool KdTree::intersect(const Ray &r, Hit &h, float tmin) {
    bool hitted = false;
    for (auto obj : bad_objects) {
        hitted |= obj->intersect(r, h, tmin);
    }
    hitted |= intersect_good(r, h, tmin);
    return hitted;
}

bool KdTree::intersect_good(const Ray &r, Hit &h, float tmin) {
    float tmax = std::numeric_limits<float>::max();
    if (!world_bound.intersect(r, tmin, tmax))
        return false;

    Vector3f inv_dir(1 / r.getDirection().x(), 1 / r.getDirection().y(), 1 / r.getDirection().z());

    std::stack<Candidate> stack;
    int id = 0;
    bool hitted = false;

    for (;;) {
        KdTreeNode &node = nodes[id];
        if (h.getT() < tmin)
            break;
        // printf("%d\n", id);
        if (node.isLeaf()) {
            int n = node.size();
            if (n == 1) {
                hitted |= objects[node.index()]->intersect(r, h, tmin);
            } else {
                for (int i = 0; i < n; ++i) {
                    int idx = indicies_pool[node.offset() + i];
                    hitted |= objects[idx]->intersect(r, h, tmin);
                }
            }

            if (stack.empty())
                break;
            id = stack.top().id;
            tmin = stack.top().tmin;
            tmax = stack.top().tmax;
            stack.pop();

        } else {
            int axis = node.splitAxis();
            float split_pos = node.splitPos();
            float origin_pos = r.getOrigin()[axis];
            float t_plane = (split_pos - origin_pos) * inv_dir[axis];

            int child0, child1;
            if ((origin_pos < split_pos) || (origin_pos == split_pos && r.getDirection()[axis] <= 0)) {
                child0 = id + 1;
                child1 = node.rchild();
            } else {
                child0 = node.rchild();
                child1 = id + 1;
            }

            if (t_plane > tmax || t_plane <= 0)
                id = child0;
            else if (t_plane < tmin)
                id = child1;
            else {
                stack.push({child1, t_plane, tmax});
                id = child0;
                tmax = t_plane;
            }
        }
    }

    return hitted;
}

void KdTree::debug() {
    debugTree(0);
}

void KdTree::debugTree(int id) {
    KdTreeNode &node = nodes[id];
    if (node.isLeaf()) {
        printf("Leaf node %d: ", id);
        int n = node.size();
        if (n == 1) {
            printf("%s\n", objects[node.index()]->getString().c_str());
        } else {
            for (int i = 0; i < n; ++i) {
                int idx = indicies_pool[node.offset() + i];
                printf("%s ", objects[idx]->getString().c_str());
            }
            printf("\n");
        }
    } else {
        printf("Interier node %d: axis=%d, split=%.2f, lchild=%d, rchild=%d\n",
               id, node.splitAxis(), node.splitPos(), id + 1, node.rchild());
        debugTree(id + 1);
        debugTree(node.rchild());
    }
}