#ifndef _RANDOM_H
#define _RANDOM_H

#include <cstdlib>
#include <ctime>
#include <cmath>

namespace Random {

void init() {
    srand(time(NULL));
}

double uniform() {
    return (double)rand() / RAND_MAX;
}

double tent() {
    double r = 2 * Random::uniform();
    return r < 1 ? sqrt(r) - 1 : 1 - sqrt(2 - r);

}

}

#endif
