/**
 * @file profile.h
 * @brief Function declarations for profiling
 *
 * @author Jeff Schornick
 * @date 2017/08/04
 **/

#ifndef __PROFILE_H__
#define __PROFILE_H__

#include "timer.h"
#include "logger.h"

typedef struct {
  uint32_t count;
  uint8_t *id;
} Profile_Result_t;

size_t id_length;

extern uint32_t profile_overhead;

void profile_calibrate(void);

#define PROFILE(ID,CODE,RESULT) {                               \
    uint32_t start_time;                                        \
    uint32_t end_time;                                          \
    int32_t result;                                             \
    LOG_STR(PROFILING_STARTED, ID);                             \
    start_time = get_usecs();                                   \
    CODE ;                                                      \
    end_time = get_usecs();                                     \
    LOG_STR(PROFILING_COMPLETED, ID);                           \
    result = end_time - start_time -profile_overhead;           \
    *RESULT = result;                                           \
    LOG_VAL(PROFILING_RESULT, (result >= 0) ? result : 0 , ID); \
}

#endif /* __PROFILE_H__ */
