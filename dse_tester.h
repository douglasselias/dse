#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

/// @todo: Move this to os.h
#define WIN32_LEAN_AND_MEAN
#include<windows.h>

///@todo: Prefix everything!!!

/// @todo: Remove unused types.
// typedef int8_t   dse_s8;
// typedef int16_t  dse_s16;
// typedef int32_t  dse_s32;
// typedef int64_t  dse_s64;
// typedef uint8_t  dse_u8;
// typedef uint16_t dse_u16;
// typedef uint32_t dse_u32;
typedef uint64_t dse_u64;

#define bool _Bool
#define true  1
#define false 0

char to_lowercase(char c) {
  if('A' <= c && c <= 'Z') {
    return c + 32;
  } else return c;
}

dse_u64 string_length(const char* string) {
  dse_u64 length = 0;
  while(*string++ != '\0') length++;
  return length;
}

void copy_string(const char* source, char* destination) {
  dse_u64 source_length = string_length(source);
  for(dse_u64 i = 0; i < source_length; i++) {
    destination[i] = source[i];
  }
}

bool has_substring(const char* haystack, const char* needle) {
  dse_u64 haystack_length = string_length(haystack);
  dse_u64 needle_length = string_length(needle);
  if(needle_length == 0) return true;
  if(haystack_length < needle_length) return false;
  dse_u64 haystack_index = 0;
  dse_u64 needle_index = 0;

  while(haystack_index < haystack_length) {
    if(to_lowercase(haystack[haystack_index]) == to_lowercase(needle[needle_index])) {
      needle_index++;
      if(needle_index < needle_length) {
        haystack_index++;
      } else return true;
    } else {
      needle_index = 0;
      haystack_index++;
    }
  }

  return false;
}

typedef void (*dse_test_function)();
#define dse_test_functions_total 1000
dse_test_function dse_functions[dse_test_functions_total] = {0};
dse_u64 dse_functions_index = 0;

char dse_suite_query[50] = {0};
char dse_test_query[50] = {0};

LONG64 dse_total_tests = 0;
LONG64 dse_total_tests_skipped = 0;
LONG64 dse_total_tests_failed = 0;

#define DSE_ASSERT(expression, ...) \
  InterlockedIncrement64(&dse_total_tests); \
  if(!(expression)) { \
    InterlockedIncrement64(&dse_total_tests_failed); \
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
  InterlockedIncrement64(&dse_total_tests_skipped); \
  InterlockedIncrement64(&dse_total_tests); \
  printf("\033[93mSKIPPED\033[0m\tLine: %s:%d\n", __FILE__, __LINE__); \

#define DSE_PRINT_RESULTS() \
  puts(""); \
  printf("Total tests:\t%lld\n", dse_total_tests); \
  printf("Total skipped:\t%lld\n", dse_total_tests_skipped); \
  printf("Total failed:\t%lld\n", dse_total_tests_failed); \
  dse_u64 dse_total_tests_passed = dse_total_tests - dse_total_tests_skipped - dse_total_tests_failed; \
  printf("Total passed:\t%lld\n", dse_total_tests_passed); \

/// @todo: Maybe I could filter the tests when generating the file, instead of having the if statement on the macro.
#define DSE_SUITE(name, code) \
  void name() { \
    if(has_substring(#name, dse_suite_query) == false) return; \
    printf("\033[95mSuite: %s\033[0m\n", #name); \
    code \
  } \

#define DSE_SUITE_TEST(name, code) \
  for(;;) { \
    if(has_substring(#name, dse_test_query) == false) break; \
    printf("\033[95m%s\033[0m\n", #name); \
    code \
    break; \
  } \

#define DSE_TEST(name, code) \
  void name() { \
    if(has_substring(#name, dse_test_query) == false) return; \
    printf("\033[96m%s\033[0m\n", #name); \
    code \
  } \


/// @todo: Move these functions to os.h
int count_threads() {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
}

HANDLE create_thread(LPTHREAD_START_ROUTINE thread_fn, void* thread_args) {
  LPSECURITY_ATTRIBUTES default_security_attr = NULL;
  int default_stack_size = 0;
  return CreateThread(default_security_attr, default_stack_size, thread_fn, thread_args, CREATE_SUSPENDED, NULL);
}

void wait_all_threads(HANDLE* thread_array, dse_u64 total_threads) {
  // int total_threads = count_threads() - 1;
  WaitForMultipleObjects((DWORD)total_threads, thread_array, true, INFINITE);
}

void start_thread(HANDLE h) {
  ResumeThread(h);
}

typedef struct {
  dse_u64 start_index;
  dse_u64 end_index;
} ThreadArgs;

DWORD WINAPI my_thread_fn(LPVOID thread_args) {
  ThreadArgs args = *(ThreadArgs*)thread_args;
  for(dse_u64 i = args.start_index; i < args.end_index; i++) {
		dse_functions[i]();
	}

  return 0;
}