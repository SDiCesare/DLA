#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

// Utils

__device__ uint random_uint(uint seed) {
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

typedef struct {
    int x;
    int y;
    int state;
    uint seed;
} Particle;


__device__ int d_width;
__device__ int d_height;
__device__ int* d_map;

__global__ void init_particles(Particle* particles, int count) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < count) {
        Particle p;
        p.seed = (uint) (idx * 4);
        p.state = 0;
        p.x = (int)(p.seed % d_width);
        p.seed = random_uint(p.seed);
        p.y = (int)(p.seed % d_height);
        particles[idx] = p;
        // printf("%d) %d (%d, %d)\n", idx, p.seed, p.x, p.y);
    }
}

__device__ void crystallize(Particle* p) {
    p->state = 1;
    int i = p->y * d_width + p->x;
    atomicAdd(&d_map[i], 1);
}

__device__ bool should_crystallize(int x, int y) {
    for (int xs = -1; xs <= 1; xs++) {
        for (int ys = -1; ys <= 1; ys++) {
            if (ys == 0 && xs == 0) {
                continue;
            }
            if (xs + x < 0 || xs + x >= d_width) {
                continue;
            }
            if (ys + y < 0 || ys + y >= d_height) {
                continue;
            }
            int j = ((y + ys) * d_width) + (x + xs);
            if (d_map[j] > 0) { // Found a Crystal
                return true;
            }
        }
    }
    return false;
}

__device__ Particle move_particle(Particle particle) {
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
    if (particle.x >= d_width) {
        particle.x = 0;
    } else if (particle.x < 0) {
        particle.x = d_width - 1;
    }
    if (particle.y >= d_height) {
        particle.y = 0;
    } else if (particle.y < 0) {
        particle.y = d_height - 1;
    }
    return particle;
}

__global__ void tick(Particle* particles, int count) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < count) {
        Particle p = particles[idx];
        if (p.state == 1) { // Crystallized Particle
            return;
        }
        if (should_crystallize(p.x, p.y)) {
            particles[idx].state = 1;
            int i = p.y * d_width + p.x;
            atomicAdd(&d_map[i], 1);
        } else {
            p = move_particle(p);
            particles[idx] = p;
        }
    }
}

void cuda_error() {
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        while (err != cudaSuccess) {
            printf("Error:\n\t%s\n", cudaGetErrorString(err));
            err = cudaGetLastError();
        }
        exit(EXIT_FAILURE);
    }
}

void save_map(char* file_name, int* map, int width, int height) {
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
    // Read Arguments
    if (argc < 6) {
        printf("Usage: main WIDTH HEIGHT PARTICLES STEPS X Y out_map\n");
        exit(EXIT_FAILURE);
    }
    int h_width = atoi(argv[1]);
    int h_height = atoi(argv[2]);
    int h_particles_count = atoi(argv[3]);
    int h_steps = atoi(argv[4]);
    int h_start_x = atoi(argv[5]);
    int h_start_y = atoi(argv[6]);
    int h_start_idx = h_start_y * h_width + h_start_x;
    char* h_out_map = "crystal.txt";
    if (argc >= 8) {
        h_out_map = argv[7];
    }
    double iStart = cpuSecond();
    // Init Map
    int* h_map = (int*)malloc(sizeof(int) * h_width * h_height);
    for (int i = 0; i < h_width * h_height; i++) {
        h_map[i] = 0;
    }
    h_map[h_start_idx] = 1; // First Crystal
    // Allocating Device Memory
    // Initializing Devices
    int* d_map_tmp;
    cudaMalloc((void**)&d_map_tmp, sizeof(int) * h_width * h_height);
    cudaMemcpy(d_map_tmp, h_map, sizeof(int) * h_width * h_height, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(d_width, &h_width, sizeof(int));
    cudaMemcpyToSymbol(d_height, &h_height, sizeof(int));
    cudaMemcpyToSymbol(d_map, &d_map_tmp, sizeof(int*));
    Particle* d_particles;
    cudaMalloc((void**)&d_particles, sizeof(int) * h_particles_count);
    cuda_error();
    // Initializing Particles
    const int base_div = 256; 
    int init_threads = base_div + (h_particles_count % base_div);
    int init_blocks = h_particles_count / base_div;
    if (h_particles_count < 256) {
        init_threads = h_particles_count;
        init_blocks = 1;
    }
    init_particles<<<init_blocks, init_threads>>>(d_particles, h_particles_count);
    cudaDeviceSynchronize();
    cuda_error();
	printf("Initialization time: %.3f milliseconds\n", 1000*(cpuSecond() - iStart));
    printf("Running Simulation\n");
    double sStart = cpuSecond();
    for (int step = 0; step < h_steps; step++) {
        // printf("Step %d\n", step);
        tick<<<init_blocks, init_threads>>>(d_particles, h_particles_count);
        // cudaDeviceSynchronize();
        cuda_error();
    }
    cudaDeviceSynchronize();
    cuda_error();
	printf("Simulation time: %.3f milliseconds\n", 1000*(cpuSecond() - sStart));
    cudaMemcpyFromSymbol(&d_map_tmp, d_map, sizeof(int *));
    cudaMemcpy(h_map, d_map_tmp, sizeof(int) * h_width * h_height, cudaMemcpyDeviceToHost);
    cuda_error();
    // Save Map 
    save_map(h_out_map, h_map, h_width, h_height);
    // Free Memory
    cudaFree(d_map_tmp);
    cudaFree(d_particles);
    free(h_map);
    cuda_error();
	printf("Total time: %.3f milliseconds\n", 1000*(cpuSecond() - iStart));
    return 0;
}