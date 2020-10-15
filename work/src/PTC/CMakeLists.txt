# Source files
set(sources
	"particles.hpp"
	"particles.cpp"
	"util.hpp"
	"util.cpp"
	"CMakeLists.txt"

)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})