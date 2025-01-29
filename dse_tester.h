#ifndef DSE_TESTER_H
#define DSE_TESTER_H

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#define WIN32_LEAN_AND_MEAN
#include<windows.h>

bool has_substring(const char* haystack, const char* needle);

bool has_substring(const char* haystack, const char* needle) {
  size_t haystack_length = strlen(haystack);
  size_t needle_length = strlen(needle);
  if(needle_length == 0) return true;
  if(haystack_length < needle_length) return false;
  size_t haystack_index = 0;
  size_t needle_index = 0;

  for(; haystack_index < haystack_length; ) {
    if(tolower(haystack[haystack_index]) == tolower(needle[needle_index])) {
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

typedef void(*dse_test_function)();
dse_test_function dse_functions[100] = {0};
int dse_function_index = 0;

void dse_register(dse_test_function f) {
  dse_functions[dse_function_index++] = f;
}

char* read_entire_file(const char* path) {
  FILE* file = fopen(path, "rb");
  if(file == NULL) puts("File 404");
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  // printf("SIZE: %zd\n", size);
  fseek(file, 0, SEEK_SET);

  char* string = malloc(size + 1);
  fread(string, size, 1, file);
  fclose(file);

  string[size] = '\0';
  return string;
}

char* dse_suite_query = "";
char* dse_test_query = "";

int dse_total_tests = 0;
int dse_total_tests_skipped = 0;
int dse_total_tests_failed = 0;
int dse_total_tests_passed = 0;

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
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    puts(""); \
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
  dse_total_tests_passed = dse_total_tests - dse_total_tests_skipped - dse_total_tests_failed; \
  printf("Total passed:\t%d\n", dse_total_tests_passed); \

#define DSE_START_SUITE(name) \
  void name() { \
    if(has_substring(#name, dse_suite_query) == false) return; \
    printf("Suite: %s\n", #name); \

#define DSE_START_TEST(name) \
  void name() { \
    if(has_substring(#name, dse_test_query) == false) return; \
    printf("Test: %s\n", #name); \

#define DSE_START_TEST_SUITE(name) \
  for(;;) { \
    if(has_substring(#name, dse_test_query) == false) break; \
    printf("Test Suite: %s\n", #name); \

#endif // DSE_TESTER_H