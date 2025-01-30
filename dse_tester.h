#ifndef DSE_TESTER_H
#define DSE_TESTER_H

#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>

/// @todo: Remove unused types.
// typedef int8_t   dse_s8;
// typedef int16_t  dse_s16;
// typedef int32_t  dse_s32;
// typedef int64_t  dse_s64;
// typedef uint8_t  dse_u8;
// typedef uint16_t dse_u16;
// typedef uint32_t dse_u32;
typedef uint64_t dse_u64;
// typedef float    dse_f32;
// typedef double   dse_f64;

char to_lowercase(char c) {
  if('A' <= c && c <= 'Z') {
    return c + 32;
  } else return c;
}

dse_u64 string_length(const char* string) {
  dse_u64 length = 0;
  while(*string != '\0') length++;
  return length;
}

bool has_substring(const char* haystack, const char* needle) {
  dse_u64 haystack_length = string_length(haystack);
  dse_u64 needle_length = string_length(needle);
  if(needle_length == 0) return true;
  if(haystack_length < needle_length) return false;
  dse_u64 haystack_index = 0;
  dse_u64 needle_index = 0;

  for(; haystack_index < haystack_length; ) {
    if(to_lowercase(haystack[haystack_index]) == to_lowercase(needle[needle_index])) {
      needle_index++;
      if(needle_index < needle_length) {
        haystack_index++;
      } else return true;
    } else {
      haystack_index++;
    }
  }

  return false;
}

typedef void (*dse_test_function)();
/// @todo: What is the maximum number possible on the stack?
#define dse_test_functions_total 1000
dse_test_function dse_functions[dse_test_functions_total] = {0};
dse_u64 dse_functions_index = 0;

/// @todo: Is this the right place for these?
char* dse_suite_query = "";
char* dse_test_query = "";

dse_u64 dse_total_tests = 0;
dse_u64 dse_total_tests_skipped = 0;
dse_u64 dse_total_tests_failed = 0;

#define DSE_ASSERT(expression, ...) \
  dse_total_tests++; \
  if(!(expression)) { \
    dse_total_tests_failed++; \
    printf("\033[31mFAILED\033[0m\t"); \
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    printf("" __VA_ARGS__); \
    puts(""); \
  } else { \
    printf("\033[32mPASSED\033[0m\t"); \
    printf("Line: %s:%d  \n", __FILE__, __LINE__); \
  } \

#define DSE_SKIP(expression) \
  if(false) { expression } \
  dse_total_tests_skipped++; \
  dse_total_tests++; \
  printf("\033[93mSKIPPED\033[0m\tLine: %s:%d\n", __FILE__, __LINE__); \

#define DSE_PRINT_RESULTS() \
  puts(""); \
  printf("Total tests:\t%d\n", dse_total_tests); \
  printf("Total skipped:\t%d\n", dse_total_tests_skipped); \
  printf("Total failed:\t%d\n", dse_total_tests_failed); \
  dse_u64 dse_total_tests_passed = dse_total_tests - dse_total_tests_skipped - dse_total_tests_failed; \
  printf("Total passed:\t%d\n", dse_total_tests_passed); \

/// @todo: Maybe I could filter the tests when generating the file, instead of having the if statement on the macro.
#define DSE_SUITE(name, code) \
  void name() { \
    if(has_substring(#name, dse_suite_query) == false) return; \
    printf("Suite: %s\n", #name); \
    code \
  } \

#define DSE_SUITE_TEST(name, code) \
  for(;;) { \
    if(has_substring(#name, dse_test_query) == false) break; \
    printf("\tSuite Test: %s\n", #name); \
    code \
    break; \
  } \

#define DSE_TEST(name, code) \
  void name() { \
    if(has_substring(#name, dse_test_query) == false) return; \
    printf("Test: %s\n", #name); \
    code \
  } \

#endif // DSE_TESTER_H