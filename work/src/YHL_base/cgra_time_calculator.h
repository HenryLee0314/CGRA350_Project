#ifndef CGRA_TIME_CALCULATOR_H
#define CGRA_TIME_CALCULATOR_H

#ifdef TIME_CALCULATOR

#include "tool_timecalculator.h"
#include "tool_module.h"

#define CGRA_ACTIVITY_START(MODULE) set_activity_start(UTILITY::MODULE)

#define CGRA_ACTIVITY_END(MODULE) set_acticity_end(UTILITY::MODULE);

#define CGRA_SHOW_SINGLE_ACTIVITY_RESULT(MODULE) show_single_activity_result(UTILITY::MODULE);

#define CGRA_SHOW_TIME_CALCULATOR_RESULT() show_time_calculator_result();

#else

#define CGRA_ACTIVITY_START(MODULE)

#define CGRA_ACTIVITY_END(MODULE)

#define CGRA_SHOW_SINGLE_ACTIVITY_RESULT(MODULE)

#define CGRA_SHOW_TIME_CALCULATOR_RESULT()

#endif // TIME_CALCULATOR

#endif // CGRA_TIME_CALCULATOR_H
