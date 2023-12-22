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

void move_particle(Particle* p) {
    random_movement(&p->x, &p->y);
}