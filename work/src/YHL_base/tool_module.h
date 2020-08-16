#ifndef TOOL_MODULE_H
#define TOOL_MODULE_H

#include <string>

namespace UTILITY {

typedef enum moduleMember_s {
    OS                 = 0,
    FILE               = 1,
    INPUT_PROCESS      = 2,
    SIGNAL_SLOT        = 3,
    CGRA409            = 4,
    MAXIMUM            = 5
} moduleMember_t;

static const std::string moduleMemberName[moduleMember_t::MAXIMUM] = {
    "OS           ",
    "FILE         ",
    "INPUT_PROCESS",
    "SIGNAL_SLOT  ",
    "CGRA350      "
};

}

#endif // TOOL_MODULE_H
