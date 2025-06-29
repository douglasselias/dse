
#include <stdlib.h>
#include <stdio.h>

#define DSE_STRING8_IMPLEMENTATION
#include "string8/string8.h"

#define DSE_INIT_TESTS()              \
  dse_u64 dse_failed_tests_count = 0; \

#define ASSERTION(expression, ...)                                   \
  if(!(expression))                                                  \
  {                                                                  \
    printf("\n\033[31mFAILED\033[0m\t%s:%d : ", __FILE__, __LINE__); \
    printf("" __VA_ARGS__);                                          \
    dse_failed_tests_count++;                                        \
  }                                                                  \

#define DSE_PRINT_ALL_TESTS_PASSED()         \
  puts("");                                  \
  if(dse_failed_tests_count == 0)            \
  {                                          \
    puts("\033[36mALL TESTS PASSED\033[0m"); \
  }                                          \

int main()
{
  DSE_INIT_TESTS();

  { // __dse_string_size function test
    dse_u64 result = __dse_string_size("hello");
    dse_u64 target = 5;
    ASSERTION(result == target, "Expected %lld, but got %lld", target, result);
  }

  { // DSE_STR8 macro test
    String8 result = DSE_STR8("hello");
    String8 target = {"hello", 5};

    ASSERTION(result.size == target.size, "Expected %lld, but got %lld", target.size, result.size);
    ASSERTION(dse_strings_are_equal(result, target), "Expected %s, but got |%s|", target.text, result.text);
  }

  {
    String8 a = DSE_STR8("hello");
    String8 b = DSE_STR8("hello");

    bool result = dse_strings_are_equal(a, b);

    ASSERTION(result == true, "Strings are not equal A: %s, B: %s", a.text, b.text);
  }

  {
    String8 a = DSE_STR8("hello");
    String8 b = DSE_STR8("hello!");

    bool result = dse_strings_are_equal(a, b);

    ASSERTION(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
  }

  {
    String8 a = DSE_STR8("hello");
    String8 b = DSE_STR8("hellw");

    bool result = dse_strings_are_equal(a, b);

    ASSERTION(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
  }

  {
    String8 input  = DSE_STR8("hello");
    String8 target = DSE_STR8("HELLO");

    String8 result = dse_to_uppercase(input);

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  {
    String8 input  = DSE_STR8("HELLO");
    String8 target = DSE_STR8("hello");

    String8 result = dse_to_lowercase(input);

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  DSE_PRINT_ALL_TESTS_PASSED();

  return 0;
}