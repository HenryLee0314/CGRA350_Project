#ifndef TOOL_TIMECALCULATOR_H
#define TOOL_TIMECALCULATOR_H

#ifdef _WIN32
#include <sys/timeb.h>
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <string>
#include "tool_module.h"

int set_activity_start(UTILITY::moduleMember_t module);

int set_acticity_end(UTILITY::moduleMember_t module);

void show_single_activity_result(UTILITY::moduleMember_t module);

void show_time_calculator_result();

unsigned long us_ticker_read();

namespace UTILITY {

class timeCalculatorActivity {

public:
    timeCalculatorActivity(std::string activity_name);

    int set_activity_start();

    int set_activity_end();

    void print_result();

protected:
    int update_time_use();

private:
    std::string activity_name;
    struct timeval activity_start;
    struct timeval activity_end;
    float activity_time_use;
    float minimum_activity_time_use;
    float maximum_activity_time_use;
    float average_activity_time_use;
    int activity_start_count;
    int activity_end_count;
};

class timeCalculator {
public:
    timeCalculator();

    ~timeCalculator();

    static timeCalculator* time_calculator;

    int set_time_calculator_start(moduleMember_t module);

    int set_time_calculator_end(moduleMember_t module);

    void print_activity_result(moduleMember_t module);

    void print_time_calculator_result();

protected:

private:
    timeCalculatorActivity* time_calculator_activity[moduleMember_t::MAXIMUM];
};

} // namespace UTILITY

#endif // TOOL_TIMECALCULATOR_H
