/**
 * @file main.c
 * @brief A simple main function that for Project 1
 *
 * Defines a simple main function which can be used as a standard entry point
 * which will call one or more project deliverables.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#include "project1.h"

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

#ifdef PROJECT1
  project1();
#endif

  return 0;
}
