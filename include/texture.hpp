#ifndef MY_TEXTURE_H
#define MY_TEXTURE_H

#include "image.hpp"
#include "utils.hpp"
#include "vecmath.h"
#include <cassert>

class Texture {
public:
    Texture() {
        image = nullptr;
    }

    Texture(const char *filename) {
        image = new Image(filename);
        assert(image != nullptr);
    }

    Texture(const Texture &texture) = delete;

    ~Texture() {
        assert(image == nullptr);
        delete image;
    }

    Vector3f getColor(const Vector2f &uv_point) const {
        int x = Utils::clamp(uv_point.x()) * (image->width() - 1);
        int y = Utils::clamp(uv_point.y()) * (image->height() - 1);
        return image->get(x, y);
    }

private:
    Image *image;
};

#endif
