# Source files
set(sources

    "grass.h"
    "grass.cpp"

    "fluid_simulation.h"
    "fluid_simulation.cpp"

    "fluid_grid.h"
    "fluid_grid.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})