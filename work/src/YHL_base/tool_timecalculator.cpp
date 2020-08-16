#include "tool_timecalculator.h"
#include "tool_heapcalculator.h"
#include "tool_log.h"

namespace UTILITY {

#ifdef _WIN32
int gettimeofday(struct timeval *tp, void *tzp) {
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#endif

timeCalculatorActivity::timeCalculatorActivity(std::string activity_name):
    activity_name(activity_name),
    activity_time_use(0),
    minimum_activity_time_use(0xffffffff),
    maximum_activity_time_use(0),
    average_activity_time_use(0),
    activity_start_count(0),
    activity_end_count(0) {
    ;
}

int timeCalculatorActivity::set_activity_start() {
    gettimeofday(&(this->activity_start), nullptr);
    ++this->activity_start_count;
    if (this->activity_start_count < 0) {
        return -1;
    } else {
        return 0;
    }
}

int timeCalculatorActivity::set_activity_end() {
    gettimeofday(&(this->activity_end), nullptr);
    ++this->activity_end_count;
    if (this->activity_end_count < 0) {
        return -1;
    } else {
        return update_time_use();
    }
}

void timeCalculatorActivity::print_result() {
    if (this->activity_start_count < this->activity_end_count) {
        UTILITY_LOGP("Activity:%s, start_count is fewer than end_count", this->activity_name.c_str());
    } else if (this->activity_start_count > this->activity_end_count) {
        UTILITY_LOGP("Activity:%s, start_count is more than end_count", this->activity_name.c_str());
    } else {
        UTILITY_LOGP("Activity: %s, Min(us):%f, Max(us):%f, Avg(us):%f, Count:%d",
               this->activity_name.c_str(),
               this->minimum_activity_time_use,
               this->maximum_activity_time_use,
               this->average_activity_time_use,
               this->activity_end_count);
    }
}

int timeCalculatorActivity::update_time_use() {
    if (this->activity_start_count != this->activity_end_count) {
        return -1;
    }
    activity_time_use = (1000000 * (this->activity_end.tv_sec - this->activity_start.tv_sec)
                         + this->activity_end.tv_usec - this->activity_start.tv_usec) / 1.0;

    if (minimum_activity_time_use > activity_time_use) {
        minimum_activity_time_use = activity_time_use;
    }

    if (maximum_activity_time_use < activity_time_use) {
        maximum_activity_time_use = activity_time_use;
    }

    average_activity_time_use = average_activity_time_use * (activity_end_count - 1);
    average_activity_time_use += activity_time_use;
    average_activity_time_use /= activity_end_count;

    return 0;
}

timeCalculator::timeCalculator()
{
    for (int i = 0; i < moduleMember_t::MAXIMUM; ++i) {
        time_calculator_activity[i] = new timeCalculatorActivity(moduleMemberName[i]);
    }
}

timeCalculator::~timeCalculator() {
    for (int i = 0; i < moduleMember_t::MAXIMUM; ++i) {
        delete(time_calculator_activity[i]);
        time_calculator_activity[i] = nullptr;
    }
}

int timeCalculator::set_time_calculator_start(moduleMember_t module) {
    return time_calculator_activity[module]->set_activity_start();
}

int timeCalculator::set_time_calculator_end(moduleMember_t module) {
    return time_calculator_activity[module]->set_activity_end();
}

void timeCalculator::print_activity_result(moduleMember_t module) {
    UTILITY_LOGP("===============time calculator=============");
    return time_calculator_activity[module]->print_result();
    UTILITY_LOGP("===========================================");
}

void timeCalculator::print_time_calculator_result() {
    UTILITY_LOGP("===============time calculator=============");
    for (int i = 0; i < moduleMember_t::MAXIMUM; ++i) {
        time_calculator_activity[i]->print_result();
    }
    UTILITY_LOGP("===========================================");
}

timeCalculator *timeCalculator::time_calculator = new timeCalculator();

} // namespace UTILITY

int set_activity_start(UTILITY::moduleMember_t module) {
    return UTILITY::timeCalculator::time_calculator->set_time_calculator_start(module);
}

int set_acticity_end(UTILITY::moduleMember_t module) {
    return UTILITY::timeCalculator::time_calculator->set_time_calculator_end(module);
}

void show_single_activity_result(UTILITY::moduleMember_t module) {
    return UTILITY::timeCalculator::time_calculator->print_activity_result(module);
}

void show_time_calculator_result() {
    return UTILITY::timeCalculator::time_calculator->print_time_calculator_result();
}

unsigned long us_ticker_read() {
    struct timeval timer;
#ifdef _WIN32
    UTILITY::gettimeofday(&(timer), nullptr);
#else
    gettimeofday(&(timer), nullptr);
#endif
    return timer.tv_sec * 1000000 + timer.tv_usec;
}
