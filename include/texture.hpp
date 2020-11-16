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
        color = Vector3f(1, 1, 1);
    }

    Texture(const Texture &texture) = delete;

    Texture(Texture &&texture) {
        image = texture.image;
        color = texture.color;
        texture.image = nullptr;
    }

    ~Texture() {
        if (image != nullptr)
            delete image;
    }

    void setColor(const Vector3f &color) {
        this->color = color;
    }

    void loadImage(const char *filename) {
        assert(image == nullptr);
        image = Image::LoadTGA(filename);
    }

    bool isConstantColor() const {
        return image == nullptr;
    }

    bool isImage() const {
        return image != nullptr;
    }

    const Vector3f &getColor() const {
        assert(isConstantColor());
        return color;
    }

    const Vector3f &getColor(const Vector2f &uv_point, const Vector3f &point) const {
        assert(isImage());

        int x = Utils::clamp(uv_point.x()) * (image->Width() - 1);
        int y = Utils::clamp(uv_point.y()) * (image->Height() - 1);
        return image->GetPixel(x, y);
    }

private:
    Image *image;
    Vector3f color;
};

#endif
