/**
 * @file timer.h
 * @brief Function declarations for time
 *
 * @author Jeff Schornick
 * @date 2017/07/22
**/

#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

static inline __attribute__((always_inline)) uint32_t get_time()
{
  struct timeval t;
  gettimeofday( &t, NULL );
  return t.tv_sec;
}

static inline __attribute__((always_inline)) uint32_t get_usecs()
{
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t );
  return t.tv_nsec / 1000;
}

static inline __attribute__((always_inline)) void delay_ms(uint32_t ms)
{
  usleep(ms*1000);
}

static inline __attribute__((always_inline)) void delay_us(uint16_t us)
{
  struct timespec t;
  t.tv_sec = 0;
  t.tv_nsec = 1000u * us;
  while( nanosleep( &t, &t ) );
}

#endif /* __TIMER_H__ */
