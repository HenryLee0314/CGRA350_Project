# Source files
set(sources
	"basic.h"
	"basic.cpp"
    "vector.h"
    "vector.cpp"
    "texture.h"
    "texture.cpp"
    "camera.h"
    "camera.cpp"
    "cgraFile.cpp"
    "cgraFile.h"
    "cgra_heap_calculator.h"
    "cgra_log.h"
    "cgra_time_calculator.h"
    "shader.cpp"
    "shader.h"
    "tool_heapcalculator.cpp"
    "tool_heapcalculator.h"
    "tool_log.h"
    "tool_module.h"
    "tool_timecalculator.cpp"
    "tool_timecalculator.h"
	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})