#ifndef MY_IMAGE_H
#define MY_IMAGE_H

#include "vecmath.h"
#include <cassert>
#include <png++/png.hpp>
#include <string>

class Image {
public:
    Image(std::string filename) : image(filename) {}

    Image(int w, int h) : image(w, h) {}

    int width() const {
        return image.get_width();
    }

    int height() const {
        return image.get_height();
    }

    Vector3f get(int x, int y) const {
        assert(x >= 0 && x < image.get_width());
        assert(y >= 0 && y < image.get_height());
        png::rgb_pixel pixel = image.get_pixel(x, height() - y - 1);
        // printf("%.3f %.3f %.3f\n", pixel.red / 255.f, pixel.green / 255.f, pixel.blue / 255.f);
        return Vector3f(pixel.red / 255.f, pixel.green / 255.f, pixel.blue / 255.f);
    }

    void set(int x, int y, const Vector3f &color) {
        assert(x >= 0 && x < image.get_width());
        assert(y >= 0 && y < image.get_height());
        // FIXME: gamma correction
        png::rgb_pixel pixel(clamp(color.x()), clamp(color.y()), clamp(color.z()));
        image.set_pixel(x, height() - y - 1, pixel);
    }

    void save(std::string filename) {
        image.write(filename);
    }

private:
    png::image<png::rgb_pixel> image;

    unsigned char clamp(float c) {
        int tmp = int(c * 255);
        if (tmp < 0)
            tmp = 0;
        if (tmp > 255)
            tmp = 255;
        return (unsigned char)tmp;
    }
};

#endif