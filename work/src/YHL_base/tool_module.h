#ifndef TOOL_MODULE_H
#define TOOL_MODULE_H

#include <string>

namespace UTILITY {

typedef enum moduleMember_s {
    OS                    = 0,
    FILE                  = 1,
    INPUT_PROCESS         = 2,
    TWO_PASS              = 3,
    CGRA350               = 4,
    GRASS_RENDER          = 5,
    GL_SET_RENDER_DATA    = 6,
    GL_SET_OPENGL_BUFFER  = 7,
    GL_RENDER_VOL_DATA    = 8,
    OPENCL_INIT           = 9,
    OPENCL_TASK           = 10,
    GRASS_UPDATE          = 11,
    GPU_FLUID_SIM         = 12,
    MAXIMUM               = 13
} moduleMember_t;

static const std::string moduleMemberName[moduleMember_t::MAXIMUM] = {
    "OS                     ",
    "FILE                   ",
    "INPUT_PROCESS          ",
    "TWO_PASS               ",
    "CGRA350                ",
    "GRASS_RENDER           ",
    "GL_SET_RENDER_DATA     ",
    "GL_SET_OPENGL_BUFFER   ",
    "GL_RENDER_VOL_DATA     ",
    "OPENCL_INIT            ",
    "OPENCL_TASK            ",
    "GRASS_UPDATE           ",
    "GPU_FLUID_SIM          "
};

}

#endif // TOOL_MODULE_H
