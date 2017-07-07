/**
 * @file main.c
 * @brief A simple main function that for Project 2
 *
 * Defines a simple main function which can be used as a standard entry point
 * which will call one or more project deliverables.
 *
 * @author Jeff Schornick
 * @date 2017/07/06
**/

#include "project2.h"

/**
 * @brief A simple main function for calling project deliverables
 *
 * A basic function which can be used as a standard entry point to call one or
 * more project deliverables. Deliverables can be selected by definining the
 * appropriate compile-time switch.
 *
 * @return Always returns zero
**/
int main(void)
{

#ifdef PROJECT2
  project2();
#endif

  return 0;
}
