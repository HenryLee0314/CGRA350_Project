#include "tool_heapcalculator.h"

namespace UTILITY {

const static size_t HEAP_BLOCK_INFO_SIZE = sizeof(heap_block_info_t);
static bool heap_calculator_init_complete = false;
static size_t static_heap_size = 0;
static size_t current_heap_size = 0;
static size_t max_heap_size = 0;
static size_t current_block_num = 0;
static size_t max_block_num = 0;

heapCalculatorActivity::heapCalculatorActivity(std::string name) :
    activity_name(name),
    max_heap_size(0),
    current_heap_size(0) {
    ;
}

int heapCalculatorActivity::increase_heap(size_t size) {
    current_heap_size += size;
    if (current_heap_size > max_heap_size) {
        max_heap_size = current_heap_size;
    }
    return 0;
}

int heapCalculatorActivity::decrease_heap(size_t size) {
    current_heap_size -= size;
    return 0;
}

void heapCalculatorActivity::print_result() {
    UTILITY_LOGP("Activity:%s, current heap:%zu, max heap:%zu",
           this->activity_name.c_str(),
           this->current_heap_size,
           this->max_heap_size);
}

heapCalculator::heapCalculator() {
    for (int i = 0; i < moduleMember_t::MAXIMUM; ++i) {
        heap_calculator_activity[i] = new heapCalculatorActivity(moduleMemberName[i]);
    }

    heap_calculator_activity[OS]->increase_heap(static_heap_size);
    heap_calculator_init_complete = true;
}

heapCalculator::~heapCalculator() {
    heap_calculator_init_complete = false;

    for (int i = 0; i < moduleMember_t::MAXIMUM; ++i) {
        delete heap_calculator_activity[i];
        heap_calculator_activity[i] = nullptr;
    }
}

void* heapCalculator::malloc_t(size_t size, moduleMember_t module) {
    void* buffer = nullptr;
    char* head = nullptr;
    heap_block_info_t* block_info = nullptr;

    buffer = malloc(size + HEAP_BLOCK_INFO_SIZE);
    head = (char*) buffer;

    buffer = (void*) (head + HEAP_BLOCK_INFO_SIZE);
    block_info = (heap_block_info_t*) head;
    block_info->size = size;
    block_info->module = module;

    if (heap_calculator_init_complete == true) {
        heap_calculator_activity[module]->increase_heap(size);
        current_heap_size += size;
        ++current_block_num;
    } else {
        static_heap_size += size;
        current_heap_size += size;
        ++current_block_num;
    }

    if (current_heap_size > max_heap_size) {
        max_heap_size = current_heap_size;
    }

    if (current_block_num > max_block_num) {
        max_block_num = current_block_num;
    }

    return buffer;
}

void* heapCalculator::calloc_t(size_t number, size_t type_size, moduleMember_t module) {
    void* buffer = nullptr;
    char* head = nullptr;
    heap_block_info_t* block_info = nullptr;

    size_t size = number * type_size;
    buffer = malloc(size + HEAP_BLOCK_INFO_SIZE);
    head = (char*) buffer;

    buffer = (void*) (head + HEAP_BLOCK_INFO_SIZE);
    memset(buffer, 0, size);

    block_info = (heap_block_info_t*) head;
    block_info->size = size;
    block_info->module = module;

    if (heap_calculator_init_complete == true) {
        heap_calculator_activity[module]->increase_heap(size);
        current_heap_size += size;
        ++current_block_num;
    } else {
        static_heap_size += size;
        current_heap_size += size;
        ++current_block_num;
    }

    if (current_heap_size > max_heap_size) {
        max_heap_size = current_heap_size;
    }

    if (current_block_num > max_block_num) {
        max_block_num = current_block_num;
    }

    return buffer;
}

void heapCalculator::free_t(void* ptr) {
    char* head = nullptr;
    moduleMember_t module;
    heap_block_info_t* block_info = nullptr;

    head = (char*) ptr;
    head -= HEAP_BLOCK_INFO_SIZE;

    block_info = (heap_block_info_t*) head;
    module = block_info->module;

    if (heap_calculator_init_complete == true) {
        heap_calculator_activity[module]->decrease_heap(block_info->size);
        current_heap_size -= block_info->size;
        --current_block_num;
    } else {
        static_heap_size -= block_info->size;
        current_heap_size -= block_info->size;
        --current_block_num;
    }

    free(head);
}

void heapCalculator::print_heap_calculator_result() {
    UTILITY_LOGP("===============heap calculator=============");
    for (int i = 0; i < moduleMember_t::MAXIMUM; ++i) {
        heap_calculator_activity[i]->print_result();
    }
    UTILITY_LOGP("cur heap:%zu, max heap:%zu, cur block:%zu, max block:%zu, block_size:%zu",
           current_heap_size, max_heap_size, current_block_num, max_block_num, sizeof(heap_block_info_t));
    UTILITY_LOGP("===========================================");
}

heapCalculator* heapCalculator::heap_calculator = new heapCalculator();

} //namespace UTILITY

void* malloc_t(size_t size, UTILITY::moduleMember_t module) {
    return UTILITY::heapCalculator::heap_calculator->malloc_t(size, module);
}

void* calloc_t(size_t number, size_t type_size, UTILITY::moduleMember_t module) {
    return UTILITY::heapCalculator::heap_calculator->calloc_t(number, type_size, module);
}

void free_t(void* ptr) {
    return UTILITY::heapCalculator::heap_calculator->free_t(ptr);
}

void* operator new (size_t size) {
    void* buffer = nullptr;
    buffer = UTILITY::heapCalculator::heap_calculator->malloc_t(size, UTILITY::OS);
    return buffer;
}

void* operator new[](size_t size) {
    void* buffer = nullptr;
    buffer = UTILITY::heapCalculator::heap_calculator->malloc_t(size, UTILITY::OS);
    return buffer;
}

void* operator new (size_t size, UTILITY::moduleMember_t module) {
    void* buffer = nullptr;
    buffer = UTILITY::heapCalculator::heap_calculator->malloc_t(size, module);
    return buffer;
}

void* operator new[](size_t size, UTILITY::moduleMember_t module) {
    void* buffer = nullptr;
    buffer = UTILITY::heapCalculator::heap_calculator->malloc_t(size, module);
    return buffer;
}

void operator delete (void* ptr) noexcept {
    if (ptr) {
        UTILITY::heapCalculator::heap_calculator->free_t(ptr);
    }
}


void operator delete[](void* ptr) noexcept {
    if (ptr) {
        UTILITY::heapCalculator::heap_calculator->free_t(ptr);
    }
}

void show_heap_calculator_result() {
    UTILITY::heapCalculator::heap_calculator->print_heap_calculator_result();
}
