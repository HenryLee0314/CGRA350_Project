#ifndef TOOL_HEAPCALCULATOR_H
#define TOOL_HEAPCALCULATOR_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "tool_log.h"
#include "tool_module.h"

void* malloc_t(size_t size, UTILITY::moduleMember_t module);

void* calloc_t(size_t number, size_t type_size, UTILITY::moduleMember_t module);

void free_t(void* ptr);

void* operator new (size_t size);
void* operator new[](size_t size);
void* operator new (size_t size, UTILITY::moduleMember_t module);
void* operator new[](size_t size, UTILITY::moduleMember_t module);
void operator delete (void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

void show_heap_calculator_result();

namespace UTILITY {

typedef struct heap_block_info_s {
    size_t size;
    moduleMember_t module;
} heap_block_info_t;

class heapCalculatorActivity {
public:
    heapCalculatorActivity(std::string name);

    int increase_heap(size_t size);

    int decrease_heap(size_t size);

    void print_result();

private:
    std::string activity_name;
    size_t max_heap_size;
    size_t current_heap_size;
};

class heapCalculator {
public:
    heapCalculator();

    ~heapCalculator();

    static heapCalculator* heap_calculator;

    void* malloc_t(size_t size, moduleMember_t module);

    void* calloc_t(size_t number, size_t type_size, moduleMember_t module);

    void free_t(void* ptr);

    void print_heap_calculator_result();


private:
    heapCalculatorActivity* heap_calculator_activity[moduleMember_t::MAXIMUM];
};

} // namespace UTILITY

#endif // TOOL_HEAPCALCULATOR_H
