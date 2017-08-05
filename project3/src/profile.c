/**
 * @file profile.c
 * @brief Profiling functions
 *
 *
 * @author Jeff Schornick
 * @date 2017/08/04
 **/

#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "timer.h"
#include "profile.h"

uint32_t profile_overhead = 0;

void profile_calibrate() {
  uint32_t start_time;
  uint32_t end_time;
  start_time = get_usecs();
  end_time = get_usecs();
  profile_overhead = end_time - start_time;
  LOG_VAL(INFO, profile_overhead, "Profile overhead");
}
