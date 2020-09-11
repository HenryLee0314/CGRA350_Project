#ifndef CPU_FLUID_SIMULATION_H
#define CPU_FLUID_SIMULATION_H

namespace CGRA350 {

struct CPU_FluidCube {
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

    float *p;
    float *div;
};
typedef struct CPU_FluidCube CPU_FluidCube;

CPU_FluidCube *CPU_FluidCubeCreate(int size, float diffusion, float viscosity, float dt);

void CPU_FluidCubeFree(CPU_FluidCube *cube);

void CPU_FluidCubeAddDensity(CPU_FluidCube *cube, int x, int y, int z, float amount);

void CPU_FluidCubeAddVelocity(CPU_FluidCube *cube, int x, int y, int z, float amountX, float amountY, float amountZ);

void CPU_FluidCubeStep(CPU_FluidCube *cube);

void CPU_FluidCubeReset(CPU_FluidCube *cube);

} // namespace CGRA350

#endif // CPU_FLUID_SIMULATION_H