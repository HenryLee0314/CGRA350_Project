# Source files
set(sources
	"sphere_model.h"
    "sphere_model.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})