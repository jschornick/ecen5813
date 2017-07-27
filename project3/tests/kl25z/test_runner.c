#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(Memory);
}

#ifdef KL25Z

int main(void)
{
#ifdef UNITY_VERBOSE
  int argc=2;
  const char *argv[] = {"", "-v"};
#else
  int argc=0;
  const char* argv[] = {};
#endif

  UnityMain(argc, argv, RunAllTests);
  while(1);
  return 0;
}

#else /* !def KL25Z */

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}

#endif
