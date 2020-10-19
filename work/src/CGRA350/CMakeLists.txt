# Source files
set(sources

    "grass.h"
    "grass.cpp"
    "grass_parameters.h"
    "grass_parameters.cpp"
    "grass_bundle.h"
    "grass_bundle.cpp"

    "object.h"
    "object.cpp"

    "floor.h"
    "floor.cpp"
    "floor_shadow.h"
    "floor_shadow.cpp"

    "light.h"
    "light.cpp"

    "shader_first_pass.h"
    "shader_first_pass.cpp"
    "shader_second_pass.h"
    "shader_second_pass.cpp"

    #"cpu_fluid_simulation.h"
    #"cpu_fluid_simulation.cpp"
    "gpu_fluid_simulation.h"
    "gpu_fluid_simulation.cpp"

    "fluid_grid.h"
    "fluid_grid.cpp"

    "opencl_manager.h"
    "opencl_manager.cpp"
    "opencl_task.h"
    "opencl_task.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})