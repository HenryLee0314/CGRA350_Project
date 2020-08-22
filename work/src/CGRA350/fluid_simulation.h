#ifndef FLUID_SIMULATION_H
#define FLUID_SIMULATION_H

#define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

namespace CGRA350 {

struct FluidCube {
    int size;
    float dt;
    float diff;
    float visc;

    float *s;
    float *density;

    float *Vx;
    float *Vy;
    float *Vz;

    float *Vx0;
    float *Vy0;
    float *Vz0;
};
typedef struct FluidCube FluidCube;

FluidCube *FluidCubeCreate(int size, float diffusion, float viscosity, float dt);

void FluidCubeFree(FluidCube *cube);

void FluidCubeAddDensity(FluidCube *cube, int x, int y, int z, float amount);

void FluidCubeAddVelocity(FluidCube *cube, int x, int y, int z, float amountX, float amountY, float amountZ);

void FluidCubeStep(FluidCube *cube);

void FluidCubeReset(FluidCube *cube);

} // namespace CGRA350

#endif // FLUID_SIMULATION_H