#ifndef MY_TEXTURE_H
#define MY_TEXTURE_H

#include "image.hpp"
#include "utils.hpp"
#include "vecmath.h"
#include <cassert>
#include <string>

class Texture {
public:
    Texture() {
        image = nullptr;
    }

    Texture(const std::string filename) {
        this->image = new Image(filename);
        this->scale = {1, 1, 1};
        assert(image != nullptr);
    }

    Texture(const std::string filename, Vector3f scale) {
        this->image = new Image(filename);
        this->scale = scale;
        assert(image != nullptr);
    }

    Texture(const Texture &texture) = delete;

    Texture(Texture &&texture) {
        assert(texture.image != nullptr);
        this->image = texture.image;
        this->scale = texture.scale;
        texture.image = nullptr;
    }

    ~Texture() {
        if (image != nullptr)
            delete image;
    }

    Vector3f getColor(const Vector2f &uv_point) const {
        assert(image != nullptr);
        int x = Utils::clamp(uv_point.x()) * (image->width() - 1);
        int y = Utils::clamp(uv_point.y()) * (image->height() - 1);
        return image->get(x, y);
    }

    Image *image;
    Vector3f scale;
};

#endif
