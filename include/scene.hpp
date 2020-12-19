#ifndef MY_SCENE_H
#define MY_SCENE_H

#include "camera.hpp"
#include "group/basic_group.hpp"
#include "material.hpp"
#include "medium.hpp"
#include "texture.hpp"
#include "vecmath.h"
#include <string>
#include <vector>

class Scene {
public:
    Scene(const std::string &scene_name, int resolution);

    ~Scene() {
        delete camera;
        if (medium != nullptr)
            delete medium;
        delete group;
        for (auto material : materials)
            delete material;
    }

    void operator=(const Scene &) = delete;

    Camera *getCamera() const {
        return camera;
    }

    Group *getGroup() const {
        return group;
    }

    Medium *getMedium() const {
        return medium;
    }

private:
    Camera *camera;
    Group *group;
    Medium *medium;
    std::vector<Texture *> textures;
    std::vector<Material *> materials;

    Texture *add(Texture *texture) {
        textures.push_back(texture);
        return texture;
    }

    Material *add(Material *material) {
        materials.push_back(material);
        return material;
    }

    Object3D *add(Object3D *object) {
        group->add(object);
        return object;
    }
};

#endif