#ifndef MY_SCENE_H
#define MY_SCENE_H

#include "camera.hpp"
#include "group/basic_group.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "vecmath.h"
#include <string>
#include <vector>

class Scene {
public:
    Scene(const std::string &scene_name, int resolution);

    ~Scene() {
        delete camera;
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

    const Vector3f &getBackgroundColor() const {
        return background_color;
    }

private:
    Camera *camera;
    Group *group;
    std::vector<Texture *> textures;
    std::vector<Material *> materials;
    Vector3f background_color;

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