#include "random_walker.h"
#include <stdlib.h>

void random_movement(int* x, int* y) {
    int s = (rand() % 2)? 1 : -1;
    if (rand() % 2) {
        *x = *x + s;
    } else {
        *y = *y + s;
    }
}

void move_particle(Particle* p, int width, int height) {
    random_movement(&p->x, &p->y);
    if (p->x >= width) {
        p->x = 0;
    } else if (p->x < 0) {
        p->x = width - 1;
    }
    if (p->y >= height) {
        p->y = 0;
    } else if(p->y < 0) {
        p->y = height - 1;
    }
}