/**
 * @file test_conversion.c
 * @brief CMocka unittests for conversion functions
 *
 * @author Jeff Schornick
 * @date 2017/07/14
 **/

#include <stdarg.h>  /* for cmocka */
#include <stddef.h>  /* for cmocka */
#include <setjmp.h>  /* for cmocka */
#include <cmocka.h>
#include <stdlib.h>
#include "conversion.h"

static int setup(void **state)
{
  uint32_t *input = malloc(sizeof(uint32_t));
  input[0] = 0x01234567;
  input[1] = 0x89ABCDEF;
  input[2] = 0xDEADBEEF;
  *state = input;
  return 0;
}
static int teardown(void **state)
{
  free(*state);
  return 0;
}

/* Verify that a NULL src pointer fails gracefully */
void test_big_to_little_null(void **state)
{
  assert_int_equal(big_to_little32(0, 1), CONVERT_FAIL);
}

/* Verify that we can convert a range of words */
void test_big_to_little_range(void **state)
{
  uint32_t *input = *state;
  uint32_t expect[] = {0x67452301, 0xEFCDAB89, 0xEFBEADDE};
  uint32_t len = 3;

  /* Convert the entire range */
  big_to_little32(input, len);

  /* Verify each word has been converted */
  for(int i=0; i<len; i++) {
    assert_int_equal(input[i], expect[i]);
  }
}

/* Verify that words outside the range aren't converted */
void test_big_to_little_middle(void **state)
{
  uint32_t *input = *state;
  uint32_t expect[] = {0x01234567, 0xEFCDAB89, 0xDEADBEEF};
  uint32_t len = 3;

  /* Only convert the middle word */
  big_to_little32(input+1, 1);

  /* Verify that only the middle word changed */
  for(int i=0; i<len; i++) {
    assert_int_equal(input[i], expect[i]);
  }
}



/* Verify that a NULL src pointer fails gracefully */
void test_little_to_big_null(void **state)
{
  assert_int_equal(little_to_big32(0, 1), CONVERT_FAIL);
}

/* Verify that we can convert a range of words */
void test_little_to_big_range(void **state)
{
  uint32_t *input = *state;
  uint32_t expect[] = {0x67452301, 0xEFCDAB89, 0xEFBEADDE};
  uint32_t len = 3;

  /* Convert the entire range */
  little_to_big32(input, len);

  /* Verify each word has been converted */
  for(int i=0; i<len; i++) {
    assert_int_equal(input[i], expect[i]);
  }
}

/* Verify that words outside the range aren't converted */
void test_little_to_big_middle(void **state)
{
  uint32_t *input = *state;
  uint32_t expect[] = {0x01234567, 0xEFCDAB89, 0xDEADBEEF};
  uint32_t len = 3;

  /* Only convert the middle word */
  little_to_big32(input+1, 1);

  /* Verify that only the middle word changed */
  for(int i=0; i<len; i++) {
    assert_int_equal(input[i], expect[i]);
  }
}

int main(void)
{
  const struct CMUnitTest tests[] =
    {
      cmocka_unit_test(test_big_to_little_null),
      cmocka_unit_test_setup_teardown(test_big_to_little_range, setup, teardown),
      cmocka_unit_test_setup_teardown(test_big_to_little_middle, setup, teardown),

      cmocka_unit_test(test_little_to_big_null),
      cmocka_unit_test_setup_teardown(test_little_to_big_middle, setup, teardown),
      cmocka_unit_test_setup_teardown(test_little_to_big_range, setup, teardown)
    };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
