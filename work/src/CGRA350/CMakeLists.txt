# Source files
set(sources

    "grass.h"
    "grass.cpp"

    "fluid_simulation.h"
    "fluid_simulation.cpp"

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