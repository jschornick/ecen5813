#include "unity_fixture.h"
#include "platform.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(memset_dma);
  RUN_TEST_GROUP(memmove_dma);
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

  platform_init();

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
