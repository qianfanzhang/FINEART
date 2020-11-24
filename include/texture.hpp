#ifndef MY_TEXTURE_H
#define MY_TEXTURE_H

#include "image.hpp"
#include "utils.hpp"
#include "vecmath.h"
#include <cassert>

class Texture {
public:
    Texture(Vector3f color) {
        this->image = nullptr;
        this->color = color;
    }

    Texture(const char *filename) {
        this->image = new Image(filename);
        this->color = Vector3f(1, 1, 1);
    }

    Texture(const Texture &texture) = delete;

    ~Texture() {
        if (image != nullptr)
            delete image;
    }

    void setColor(const Vector3f &color) {
        this->color = color;
    }

    void loadImage(const char *filename) {
        assert(image == nullptr);
        image = new Image(filename);
    }

    bool isConstantColor() const {
        return image == nullptr;
    }

    bool isImage() const {
        return image != nullptr;
    }

    Vector3f getColor() const {
        assert(isConstantColor());
        return color;
    }

    Vector3f getColor(const Vector2f &uv_point) const {
        assert(isImage());

        int x = Utils::clamp(uv_point.x()) * (image->width() - 1);
        int y = Utils::clamp(uv_point.y()) * (image->height() - 1);
        return image->get(x, y);
    }

private:
    Image *image;
    Vector3f color;
};

#endif
