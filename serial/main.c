#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random_walker.h"
#include "particle.h"
#include "grid.h"

#define PARTICLES 25

int main(int argc, char** argv) {
    srand(time(NULL));
    Grid grid;
    printf("Initializing Grid...\n");
    init_grid(&grid, 100, 100, PARTICLES);
    print_grid(&grid);
    int stopped = 0;
    int step = 0;
    printf("Running Simulation...\n");
    while (stopped < PARTICLES) {
        for (int i = 0; i < grid.p_count; i++) {
            Particle* p = &grid.particles[i];
            if (p->state == CRYSTALLIZED) {
                continue;
            }
            int oldX = p->x, oldY = p->y;
            move_particle(p, grid.width, grid.height);
            printf("%d) Moved to (%d, %d)\n", i, p->x, p->y);
            int newPosState = grid.grid_states[p->x][p->y];
            if (newPosState >= 0) {
                printf("%d) Crystallized!\n", i);
                p->state = CRYSTALLIZED;
                p->x = oldX;
                p->y = oldY;
                stopped++;
                p = &grid.particles[newPosState];
                if (p->state == MOVING) {
                    p->state = CRYSTALLIZED;
                    stopped++;
                    printf("%d) Crystallized!\n", newPosState);
                }
            } else {
                grid.grid_states[oldX][oldY] = -1;
                grid.grid_states[p->x][p->y] = i;
            }
            // print_grid(&grid);
        }
        print_grid_step(&grid, step);
        step++;
    }
    print_grid(&grid);
    /*Particle* p = (Particle*)malloc(sizeof(Particle));
    p->x = 0;
    p->y = 0;
    while (p->state == MOVING) {
        printf("(%d, %d)\n", p->x, p->y);
        move_particle(p);
        if (p->x == -5 || p->y == -5 || p->x == 5 || p->y == -5) {
            p->state = CRYSTALLIZED;
        }
    }
    printf("(%d, %d)\n", p->x, p->y);*/
    return 0;
}
