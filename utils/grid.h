#ifndef GRID
#define GRID

#include <stdlib.h>
#include "particle.h"
#include "random_walker.h"

typedef struct Grid {
    int width;
    int height;
    Particle* particles;
    int p_count;
    int** grid_states;
} Grid;

void init_grid(Grid* grid, int width, int height, int p_count) {
    grid->width = width;
    grid->height = height;
    grid->p_count = p_count;
    // Allocate grid states
    grid->grid_states = (int**)malloc(sizeof(int*) * width);
    for (int i = 0; i < width; i++) {
        grid->grid_states[i] = (int*)malloc(sizeof(int) * height);
        for (int j = 0; j < height; j++) {
            grid->grid_states[i][j] = -1;
        }
    }
    grid->particles = (Particle*)malloc(sizeof(Particle) * p_count);
    for(int i = 0; i < p_count; i++) {
        int x = (rand() % width);
        int y = (rand() % height);
        if (grid->grid_states[x][y] >= 0) {// Regenerate random location
            i--;
            continue;
        }
        grid->particles[i].x = x;
        grid->particles[i].y = y;
        grid->particles[i].state = MOVING;
        grid->grid_states[x][y] = i;
    }
}

void print_grid(Grid* grid) {
    printf("[");
    for(int i = 0; i < grid->p_count; i++) {
        printf("(%d, %d)", grid->particles[i].x, grid->particles[i].y);
        if (i < grid->p_count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    for(int i = 0; i < grid->width; i++) {
        for(int j = 0; j < grid->height; j++) {
            printf("%d", grid->grid_states[i][j]);
            if (j < grid->height - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void print_grid_step(Grid* grid, int step) {
    FILE *fp;
    char filename[100];
    sprintf(filename, "steps/step%d.csv", step);
    if((fp=fopen(filename, "wt"))==NULL) {
		printf("Can't save step %d!\n", step);
		return;
	}
    for(int i = 0; i < grid->width; i++) {
        for(int j = 0; j < grid->height; j++) {
            fprintf(fp, "%d", grid->grid_states[i][j]);
            if (j < grid->height - 1) {
                fprintf(fp, ", ");
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Step %d saved!\n", step);
}

#endif