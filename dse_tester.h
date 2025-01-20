#ifndef DSE_TESTER_H
#define DSE_TESTER_H

#ifdef DSE_TESTER_IMPLEMENTATION

int total_tests = 0;
int total_tests_skipped = 0;
int total_tests_failed = 0;
int total_tests_passed = 0;

#define DSE_ASSERT(expression, ...) \
  total_tests++; \
  if(!(expression)) { \
    total_tests_failed++; \
    printf("\033[31mFAILED\033[0m\t"); \
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    printf("" __VA_ARGS__); \
    puts(""); \
  } else { \
    printf("\033[32mPASSED\033[0m\t"); \
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    puts(""); \
  } \

#define DSE_SKIP(expression) \
  if(false) { expression } \
  total_tests_skipped++; \
  total_tests++; \
  printf("\033[93mSKIPPED\033[0m\tLine: %s:%d\n", __FILE__, __LINE__); \

#define DSE_PRINT_RESULTS() \
  printf("Total tests: %d\n", total_tests); \
  printf("Total skipped: %d\n", total_tests_skipped); \
  printf("Total failed: %d\n", total_tests_failed); \
  total_tests_passed = total_tests - total_tests_skipped - total_tests_failed; \
  printf("Total passed: %d\n", total_tests_passed); \

#define DSE_TIMER(function) \
  clock_t start = clock(); \
  function(); \
  clock_t end = clock(); \
  printf("%s was executed in %2.3f seconds\n", #function, (double)(end - start) / CLOCKS_PER_SEC); \

#define DSE_START_SUITE(name) \
  void name() { \
    printf("Suite: %s\n", #name); \

#define DSE_END_SUITE() }

#endif // DSE_TESTER_IMPLEMENTATION
#endif // DSE_TESTER_H