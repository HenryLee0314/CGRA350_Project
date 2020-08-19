# Source files
set(sources

    "grass.h"
    "grass.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})