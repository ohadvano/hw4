#include "util.h"

#ifndef BASIC_H_
#define BASIC_H_

CYCLES main_memory_access_latency();
CYCLES first_level_cache_access_latency();
CYCLES latency_calculator(bool is_cache);

#endif
