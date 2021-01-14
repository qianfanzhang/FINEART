#ifndef MY_IMAGE_H
#define MY_IMAGE_H

#include "tinyexr.hpp"
#include "vecmath.h"
#include <cassert>
#include <png++/png.hpp>
#include <string>

class Image {
public:
    virtual ~Image() {}

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual Vector3f get(int x, int y) const = 0;
};

class PngImage : public Image {
public:
    PngImage(std::string filename) : image(filename) {}

    PngImage(int w, int h) : image(w, h) {}

    int width() const override {
        return image.get_width();
    }

    int height() const override {
        return image.get_height();
    }

    Vector3f get(int x, int y) const override {
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
        png::rgb_pixel pixel((char)(Utils::gamma_corr(color.x())), 
                             (char)(Utils::gamma_corr(color.y())), 
                             (char)(Utils::gamma_corr(color.z())));
        image.set_pixel(x, height() - y - 1, pixel);
    }

    void save(std::string filename) {
        image.write(filename);
    }

private:
    png::image<png::rgb_pixel> image;
};

class ExrImage : public Image {
public:
    ExrImage(const std::string &filename) {
        err = nullptr;
        int ret = LoadEXR(&out, &_width, &_height, filename.c_str(), &err);
        if (ret != TINYEXR_SUCCESS) {
            if (err != nullptr) {
                std::cout << "[ExrImage] tinyexr error: " << err << std::endl;
                FreeEXRErrorMessage(err);
                abort();
            }
        }
    }

    ~ExrImage() {
        free(out);
    }

    int width() const override {
        return _width;
    }

    int height() const override {
        return _height;
    }

    Vector3f get(int x, int y) const override {
        assert(x >= 0 && x < _width);
        assert(y >= 0 && y < _height);
        int idx = x + (_height - y - 1) * _width;
        return Vector3f(out[4 * idx + 0], out[4 * idx + 1], out[4 * idx + 2]);
    }

private:
    float *out; // width * height * RGBA
    int _width;
    int _height;
    const char *err;
};

inline Image *load_image(const std::string &filename) {
    if (filename.substr(std::max(4, (int)filename.size()) - 4) == std::string(".exr"))
        return new ExrImage(filename);
    return new PngImage(filename);
}

#endif