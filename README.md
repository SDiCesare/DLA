# DLA
Diffusion-limited aggregation in C using CUDA and openMP

# TODO
Cuda and PThread version:

For Cuda (Threads on Tick, or Ticks on Thread): {
    - 1D indexes (blocks & threads)
    - Crystals saved on shared memory (__device__ grid)
    - Each thread controls a particle, and moves it around: {
        - Multiple particles can be in the same grid position (x, y)
        - When a particle hits the Crystals, It is added to the crystal and removed from particles
    }
    - Threads are lunched each tick from the host, depending on the number of particles {
        - Calculated at runtime the number of threads per block, and the number of blocks
        - #b = #p / #tMax
        - #t = #tMax
        - Use the maximum threads per block to calculate the number of blocks to lunch.
    }, {
        - Get dim3 block and grid from cmd args
    }
    - Concurrency problems when on hit? {
        - Multiple particles on a single point and near a crystal
        - Problem with density -> Less density more conflict we encounter {
            - Two particles crystallize in the same position (Use different number with atomicAdd -> error handling)
        }
    }
}