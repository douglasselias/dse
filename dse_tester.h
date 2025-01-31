#include<stdio.h>
#include<stdlib.h>

#include "os/os.h"

///@todo: Prefix everything!!!

#define dse_bool _Bool
#define dse_true  1
#define dse_false 0

char dse_to_lowercase(char c) {
  if('A' <= c && c <= 'Z') return c + 32;
  else return c;
}

dse_u64 dse_string_length(const char* string) {
  dse_u64 length = 0;
  while(*string++ != '\0') length++;
  return length;
}

void dse_copy_string(const char* source, char* destination) {
  dse_u64 source_length = dse_string_length(source);
  for(dse_u64 i = 0; i < source_length; i++) {
    destination[i] = source[i];
  }
}

dse_bool dse_has_substring(const char* haystack, const char* needle) {
  dse_u64 haystack_length = dse_string_length(haystack);
  dse_u64 needle_length = dse_string_length(needle);
  if(needle_length == 0) return dse_true;
  if(haystack_length < needle_length) return dse_false;
  dse_u64 haystack_index = 0;
  dse_u64 needle_index = 0;

  while(haystack_index < haystack_length) {
    if(dse_to_lowercase(haystack[haystack_index]) == dse_to_lowercase(needle[needle_index])) {
      needle_index++;
      if(needle_index < needle_length) {
        haystack_index++;
      } else return dse_true;
    } else {
      needle_index = 0;
      haystack_index++;
    }
  }

  return dse_false;
}

dse_atomic_s64 dse_total_tests = {0};
dse_atomic_s64 dse_total_tests_skipped = {0};
dse_atomic_s64 dse_total_tests_failed = {0};

#define DSE_ASSERT(expression, ...) \
  dse_atomic_increment(dse_total_tests); \
  if(expression) { \
    printf("\033[32mPASSED\033[0m\t"); \
    printf("Line: %s:%d  \n", __FILE__, __LINE__); \
  } else { \
    dse_atomic_increment(dse_total_tests_failed); \
    printf("\033[31mFAILED\033[0m\t"); \
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    printf("" __VA_ARGS__); \
    puts(""); \
  } \

#define DSE_SKIP(expression) \
  if(dse_false) { expression } \
  dse_atomic_increment(dse_total_tests_skipped); \
  dse_atomic_increment(dse_total_tests); \
  printf("\033[93mSKIPPED\033[0m\tLine: %s:%d\n", __FILE__, __LINE__); \

#define DSE_PRINT_RESULTS() \
  puts(""); \
  printf("Total tests:\t%lld\n", dse_total_tests.value); \
  printf("Total skipped:\t%lld\n", dse_total_tests_skipped.value); \
  printf("Total failed:\t%lld\n", dse_total_tests_failed.value); \
  dse_u64 dse_total_tests_passed = dse_total_tests.value - dse_total_tests_skipped.value - dse_total_tests_failed.value; \
  printf("Total passed:\t%lld\n", dse_total_tests_passed); \

char dse_suite_query[50] = {0};
char dse_test_query[50] = {0};

/// @todo: Maybe I could filter the tests when generating the file, instead of having the if statement on the macro.
#define DSE_SUITE(name, code) \
  void name() { \
    if(dse_has_substring(#name, dse_suite_query) == dse_false) return; \
    printf("\033[95mSuite: %s\033[0m\n", #name); \
    code \
  } \

#define DSE_SUITE_TEST(name, code) \
  for(;;) { \
    if(dse_has_substring(#name, dse_test_query) == dse_false) break; \
    printf("\033[95m%s\033[0m\n", #name); \
    code \
    break; \
  } \

#define DSE_TEST(name, code) \
  void name() { \
    if(dse_has_substring(#name, dse_test_query) == dse_false) return; \
    printf("\033[96m%s\033[0m\n", #name); \
    code \
  } \

typedef void (*dse_test_function)();
#define dse_test_functions_total 1000
dse_test_function dse_functions[dse_test_functions_total] = {0};
dse_u64 dse_functions_index = 0;

/// @todo: Move these functions to os.h
dse_u64 tests_per_thread = 1;
dse_u64 remaining_tests = 0;

typedef struct {
  dse_u64 start_index;
  dse_u64 end_index;
} ThreadArgs;

dse_u64 my_thread_fn(void* thread_args) {
  ThreadArgs args = *(ThreadArgs*)thread_args;
  for(dse_u64 i = args.start_index; i < args.end_index; i++) {
		dse_functions[i]();
	}

  return 0;
}

// Global exception handler: WIP
// LONG CALLBACK VectoredHandler(PEXCEPTION_POINTERS ep) {
//   if (ep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
//     fprintf(stderr, "[ERROR] Null dereference at address 0x%p\n", (void*)ep->ExceptionRecord->ExceptionInformation[1]);
//   }
//   return EXCEPTION_CONTINUE_SEARCH; // Let other handlers process it
// }