#ifndef CGRA_LOG_H
#define CGRA_LOG_H

#include "tool_log.h"

#define CGRA_LOGE(format, ...) UTILITY_LOGE(format, ##__VA_ARGS__)
#define CGRA_LOGW(format, ...) UTILITY_LOGW(format, ##__VA_ARGS__)
#define CGRA_LOGD(format, ...) UTILITY_LOGD(format, ##__VA_ARGS__)

#define CGRA_LOGP(format, ...) UTILITY_LOGP(format "\n",##__VA_ARGS__)

#endif // CGRA_LOG_H
