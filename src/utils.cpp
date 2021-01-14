#include "utils.hpp"
#include "image.hpp"

void Utils::save_image(int num_samples, int width, int height, Vector3f rgb[][Utils::MAX_RESOLUTION], const std::string &filename) {
    PngImage image(width, height);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            image.set(i, j, rgb[i][j] / num_samples);
        }
    }
    image.save(filename.c_str());
}

void Utils::save_checkpoint(int num_samples, int width, int height, Vector3f rgb[][Utils::MAX_RESOLUTION], const std::string &filename) {
    FILE *f = fopen(filename.c_str(), "w");
    if (f == nullptr) {
        std::cout << "[Utils] save_checkpoint: %s does not exist\n"
                  << std::endl;
        abort();
    }

    fprintf(f, "%d\n", num_samples);
    fprintf(f, "%d %d\n", width, height);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            fprintf(f, "%.2f %.2f %.2f ", rgb[i][j][0], rgb[i][j][1], rgb[i][j][2]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void Utils::load_checkpoint(int &num_samples, int &width, int &height, Vector3f rgb[][Utils::MAX_RESOLUTION], const std::string &filename) {
    FILE *f = fopen(filename.c_str(), "r");
    if (f == nullptr) {
        std::cout << "[Utils] load_checkpoint: %s does not exist\n"
                  << std::endl;
        abort();
    }

    int _s, _w, _h;
    fscanf(f, "%d", &_s);
    fscanf(f, "%d %d", &_w, &_h);

    if (width != 0 && _w != width) {
        std::cout << "[Utils] load_checkpoint: width mismatch\n"
                  << std::endl;
        abort();
    }
    if (width != 0 && _w != width) {
        std::cout << "[Utils] load_checkpoint: height mismatch\n"
                  << std::endl;
        abort();
    }
    num_samples += _s;
    width = _w;
    height = _h;

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float r, g, b;
            fscanf(f, "%f%f%f", &r, &g, &b);
            rgb[i][j][0] += r;
            rgb[i][j][1] += g;
            rgb[i][j][2] += b;
        }
    }

    fclose(f);
}
