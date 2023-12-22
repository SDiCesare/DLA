#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random_walker.h"
#include "particle.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    Particle* p = (Particle*)malloc(sizeof(Particle));
    p->x = 0;
    p->y = 0;
    while (p->state == MOVING) {
        printf("(%d, %d)\n", p->x, p->y);
        move_particle(p);
        if (p->x == -5 || p->y == -5 || p->x == 5 || p->y == -5) {
            p->state = CRYSTALLIZED;
        }
    }
    printf("(%d, %d)\n", p->x, p->y);
    return 0;
}
