#ifndef TOOL_MODULE_H
#define TOOL_MODULE_H

#include <string>

namespace UTILITY {

typedef enum moduleMember_s {
    OS                    = 0,
    FILE                  = 1,
    INPUT_PROCESS         = 2,
    SIGNAL_SLOT           = 3,
    CGRA350               = 4,
    CALCULATE_VOL         = 5,
    GL_SET_RENDER_DATA    = 6,
    GL_SET_OPENGL_BUFFER  = 7,
    GL_RENDER_VOL_DATA    = 8,
    OPENCL_INIT           = 9,
    OPENCL_TASK           = 10,
    CPU_FLUID_SIM         = 11,
    GPU_FLUID_SIM         = 12,
    MAXIMUM               = 13
} moduleMember_t;

static const std::string moduleMemberName[moduleMember_t::MAXIMUM] = {
    "OS                     ",
    "FILE                   ",
    "INPUT_PROCESS          ",
    "SIGNAL_SLOT            ",
    "CGRA350                ",
    "CALCULATE_VOL          ",
    "GL_SET_RENDER_DATA     ",
    "GL_SET_OPENGL_BUFFER   ",
    "GL_RENDER_VOL_DATA     ",
    "OPENCL_INIT            ",
    "OPENCL_TASK            ",
    "CPU_FLUID_SIM          ",
    "GPU_FLUID_SIM          "
};

}

#endif // TOOL_MODULE_H
