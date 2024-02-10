# Compiler and Flags
CC = gcc
NVCC = nvcc
CCFLAGS = -Wall -Wextra -O2
NVCCFLAGS = -O2

# Targets
TARGETS = cuda serial openmp

# Default Target
all: $(TARGETS)

# Cuda Executable
cuda: cuda/main.cu
	$(NVCC) $(NVCCFLAGS) -o bin/cuda_dla $^

# Serial Executable
serial: serial/main.c
	$(CC) $(CCFLAGS) -o bin/serial_dla $^
# Openmp Executable
openmp: openmp/main.c
	$(CC) $(CCFLAGS) -fopenmp -o bin/openmp_dla $^

clean:
	rm -f bin/cuda_dla bin/serial_dla bin/openmp_dla

.PHONY: all clean cuda serial openmp