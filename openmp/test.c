#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>

uint random_uint(uint seed) {
    uint x = seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

double cpuSecond() {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}

// The sizeof Particle is 16. 64/16 = 4. Should avoid False Sharing
typedef struct {
    int x;
    int y;
    int state;
    uint seed;
} Particle;

int width;
int height;
int* map;

void init_particles(Particle* particles, int count) {
    int ids = omp_get_thread_num();
    int threads = omp_get_num_threads();
    int size = count / threads;
#   pragma omp for schedule(static, 4)
    for (int i = ids * size; i < ids * size + size; i++) {
        Particle p;
        p.seed = (uint) (i * 4);
        p.state = 0;
        p.x = (int)(p.seed % width);
        p.seed = random_uint(p.seed);
        p.y = (int)(p.seed % height);
        particles[i] = p;
    }
}

/*void init_particles(Particle* particles, int count) {
#   pragma omp parallel for schedule(static, 4)
    for (int i = 0; i < count; i++) {
        Particle p;
        p.seed = (uint) (i * 4);
        p.state = 0;
        p.x = (int)(p.seed % width);
        p.seed = random_uint(p.seed);
        p.y = (int)(p.seed % height);
        particles[i] = p;
        // printf("%d) %d (%d, %d)\n", i, p.seed, p.x, p.y);
    }
}*/

void crystallize(Particle* p) {
    p->state = 1;
    int i = p->y * width + p->x;
    map[i] += 1;
}

bool should_crystallize(int x, int y) {
    for (int xs = -1; xs <= 1; xs++) {
        for (int ys = -1; ys <= 1; ys++) {
            if (ys == 0 && xs == 0) {
                continue;
            }
            if (xs + x < 0 || xs + x >= width) {
                continue;
            }
            if (ys + y < 0 || ys + y >= height) {
                continue;
            }
            int j = ((y + ys) * width) + (x + xs);
            if (map[j] > 0) { // Found a Crystal
                return true;
            }
        }
    }
    return false;
}

Particle move_particle(Particle particle) {
    particle.seed = random_uint(particle.seed);
    int mov = particle.seed % 4;
    switch (mov) {
        case 0: 
            particle.x += 1;
            break;
        case 1: 
            particle.x -= 1;
            break;
        case 2: 
            particle.y += 1;
            break;
        case 3: 
            particle.y -= 1;
            break;
    }
    // Check Bounds
    if (particle.x >= width) {
        particle.x = 0;
    } else if (particle.x < 0) {
        particle.x = width - 1;
    }
    if (particle.y >= height) {
        particle.y = 0;
    } else if (particle.y < 0) {
        particle.y = height - 1;
    }
    return particle;
}

void move_particles(Particle* particles, int count) {
    int ids = omp_get_thread_num();
    int threads = omp_get_num_threads();
    int size = count / threads;
#   pragma omp for schedule(static, 4)
    for (int i = ids * size; i < ids * size + size; i++) {
        Particle p = particles[i];
        if (p.state == 1) {
            continue;
        }
        bool b;
#       pragma omp critical
        {
            b = should_crystallize(p.x, p.y);
        }
        if (b) {
            p.state = 1;
            int idx = p.y * width + p.x;
            map[idx] += 1;
            particles[i] = p;
        } else {
            particles[i] = move_particle(p);
        }
    }
}

void save_map(char* file_name) {
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
        printf("Error opening the file %s", file_name);
        return;
    }
    for (int i = 0; i < width * height; i++) {
        if (i % width == 0) {
            fprintf(fp, "\n");
        }
        fprintf(fp, "%d ", map[i]);
    }
    fclose(fp);
}


int main(int argc, char* argv[]) {
    omp_set_dynamic(1); // Improving Times?
    // Read Arguments
    if (argc < 6) {
        printf("Usage: main WIDTH HEIGHT PARTICLES STEPS X Y out_map\n");
        exit(EXIT_FAILURE);
    }
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    int particles_count = atoi(argv[3]);
    int steps = atoi(argv[4]);
    int start_x = atoi(argv[5]);
    int start_y = atoi(argv[6]);
    int start_idx = start_y * width + start_x;
    char* out_map = "crystal.txt";
    if (argc >= 8) {
        out_map = argv[7];
    }
    double iStart = cpuSecond();
    // Init Map
    map = (int*)malloc(sizeof(int) * width * height);
    for (int i = 0; i < width * height; i++) {
        map[i] = 0;
    }
    map[start_idx] = 1;
    // Init Particles
    Particle* particles = (Particle*)malloc(sizeof(Particle) * particles_count);
#   pragma omp parallel
    init_particles(particles, particles_count);
	printf("Initialization time: %.3f milliseconds\n", 1000*(cpuSecond() - iStart));
    // Simulate
    printf("Running Simulation\n");
    double sStart = cpuSecond();
    for (int i = 0; i < steps; i++) {
#       pragma omp parallel
        move_particles(particles, particles_count);
    }
	printf("Simulation time: %.3f milliseconds\n", 1000*(cpuSecond() - sStart));
    // Save to file
    save_map(out_map);
    // Dealloc
    free(map);
    free(particles);
	printf("Total time: %.3f milliseconds\n", 1000*(cpuSecond() - iStart));
    return 0;
}