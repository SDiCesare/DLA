#ifndef PARTICLE
#define PARTICLE

enum ParticleState {
    MOVING,
    CRYSTALLIZED
};

typedef struct Particle {
    int x;
    int y;
    enum ParticleState state;
} Particle;

#endif