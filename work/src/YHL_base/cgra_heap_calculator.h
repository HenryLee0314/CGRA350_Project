#ifndef CGRA_HEAP_CALCULATOR_H
#define CGRA_HEAP_CALCULATOR_H

#ifdef HEAP_CALCULATOR

#include "tool_heapcalculator.h"
#include "tool_module.h"

#define CGRA_MALLOC(size, module) malloc_t(size, UTILITY::module)
#define CGRA_CALLOC(number, type_size, module) calloc_t(number, type_size, UTILITY::module)
#define CGRA_FREE(ptr) free_t(ptr)
#define CGRA_NEW(module) new (UTILITY::module)
#define CGRA_SHOW_HEAP_CALCULATOR_RESULT() show_heap_calculator_result()

#else

#include <stdlib.h>

#define CGRA_MALLOC(size, module) malloc(size)
#define CGRA_CALLOC(number, type_size, module) calloc(number, type_size)
#define CGRA_FREE(ptr) free(ptr)
#define CGRA_NEW(module) new
#define CGRA_SHOW_HEAP_CALCULATOR_RESULT()

#endif // HEAP_CALCULATOR

#endif // CGRA_HEAP_CALCULATOR_H
