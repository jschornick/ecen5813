#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "conversion.h"

typedef enum {TEST_PASS, TEST_FAIL} result_t;
const char *result_str[] = {"pass", "FAIL"};

result_t test_big_to_little() {
  uint32_t input[] = {0x01234567, 0x89ABCDEF, 0xDEADBEEF};
  uint32_t expect[] = {0x67452301, 0xEFCDAB89, 0xEFBEADDE};
  uint32_t len = 3;
  
  big_to_little32(input, len);

  result_t result = TEST_PASS;
  for(int i=0; i<len; i++) {
      if(input[i] != expect[i]) {
          printf("  %x != %x\n", input[i], expect[i]);
          result = TEST_FAIL;
      }
  }
  return result;
}

result_t test_little_to_big() {
  uint32_t input[] = {0x01234567, 0x89ABCDEF, 0xDEADBEEF};
  uint32_t expect[] = {0x67452301, 0xEFCDAB89, 0xEFBEADDE};
  uint32_t len = 3;
  
  little_to_big32(input, len);

  result_t result = TEST_PASS;
  for(int i=0; i<len; i++) {
      if(input[i] != expect[i]) {
          printf("  %x != %x\n", input[i], expect[i]);
          result = TEST_FAIL;
      }
  }
  return result;
}

result_t test_big_to_little_single() {
  uint32_t input[] = {0x01234567, 0x89ABCDEF, 0xDEADBEEF};
  uint32_t expect[] = {0x01234567, 0xEFCDAB89, 0xDEADBEEF};
  uint32_t len = 3;
  
  big_to_little32(input+1, 1);

  result_t result = TEST_PASS;
  for(int i=0; i<len; i++) {
      if(input[i] != expect[i]) {
          printf("  %x != %x\n", input[i], expect[i]);
          result = TEST_FAIL;
      }
  }
  return result;
}

result_t test_big_to_little_null() {
  result_t result = TEST_PASS;
  if(big_to_little32(0, 1) == CONVERT_SUCCESS) {
    result = TEST_FAIL;
  }
  return result;
}

result_t test_atoi_binary() {
  result_t result = TEST_PASS;
  int32_t converted;
  uint8_t input[] = "-1111111111111111111111111111111";
  int32_t expected = -2147483647;
  converted = my_atoi(input, 33, BASE_2);
  if(converted != expected) {
    printf("%s --> %d != %d\n", input, converted, expected); 
    result = TEST_FAIL;
  }
  uint8_t input2[] = "-10000000000000000000000000000000";
  int32_t expected2 = -2147483648;
  converted = my_atoi(input2, 34, BASE_2);
  if(converted != expected2) {
    printf("%s --> %d != %d\n", input2, converted, expected2); 
    result = TEST_FAIL;
  }
  uint8_t input3[] = "-0";
  int32_t expected3 = 0;
  converted = my_atoi(input3, 3, BASE_2);
  if(converted != expected3) {
    printf("%s --> %d != %d\n", input3, converted, expected3); 
    result = TEST_FAIL;
  }
  return result;
}

result_t test_itoa_binary() {
  result_t result = TEST_PASS;
  uint8_t converted[34];
  int32_t input = -2147483647;
  uint8_t expected[] = "-1111111111111111111111111111111";
  my_itoa(input, converted, BASE_2);
  if( strcmp( (char *) converted, (char *) expected) ) {
    printf("%d --> %s != %s\n", input, converted, expected); 
    result = TEST_FAIL;
  }

  int32_t input2 = -2147483648;
  uint8_t expected2[] = "-10000000000000000000000000000000";
  my_itoa(input2, converted, BASE_2);
  if( strcmp( (char *) converted, (char *) expected2) ) {
    printf("%d --> %s != %s\n", input2, converted, expected2); 
    result = TEST_FAIL;
  }

  int32_t input3 = 0;
  uint8_t expected3[] = "0";
  my_itoa(input3, converted, BASE_2);
  if( strcmp( (char *) converted, (char *) expected3) ) {
    printf("%d --> %s != %s\n", input3, converted, expected3); 
    result = TEST_FAIL;
  }
  return result;
}

int main(void)
{
  printf( "[%s] big_to_little\n", result_str[test_big_to_little()]);
  printf( "[%s] little_to_big\n", result_str[test_little_to_big()]);
  printf( "[%s] big_to_little_single\n", result_str[test_big_to_little_single()]);
  printf( "[%s] big_to_little_null\n", result_str[test_big_to_little_null()]);
  printf( "[%s] atoi_binary\n", result_str[test_atoi_binary()]);
  printf( "[%s] itoa_binary\n", result_str[test_itoa_binary()]);
  return 0;
}
