#ifndef TOOL_MODULE_H
#define TOOL_MODULE_H

#include <string>

namespace UTILITY {

typedef enum moduleMember_s {
    OS                 = 0,
    FILE               = 1,
    INPUT_PROCESS      = 2,
    SIGNAL_SLOT        = 3,
    CGRA350            = 4,
    CALCULATE_VOL      = 5,
    SET_RENDER_DATA    = 6,
    SET_OPENGL_BUFFER  = 7,
    RENDER_VOL_DATA    = 8,
    MAXIMUM            = 9
} moduleMember_t;

static const std::string moduleMemberName[moduleMember_t::MAXIMUM] = {
    "OS                  ",
    "FILE                ",
    "INPUT_PROCESS       ",
    "SIGNAL_SLOT         ",
    "CGRA350             ",
    "CALCULATE_VOL       ",
    "SET_RENDER_DATA     ",
    "SET_OPENGL_BUFFER   ",
    "RENDER_VOL_DATA     "
};

}

#endif // TOOL_MODULE_H
