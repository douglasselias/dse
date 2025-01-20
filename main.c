#include<stdio.h>
#include<stdbool.h>
#include<time.h>

#define DSE_TESTER_IMPLEMENTATION
#include "dse_tester.h"

#include "code.c"

DSE_START_SUITE(sum_suite);
puts("Setup();");

{
  int result = sum(1, 2);
  DSE_ASSERT(result == 1, "Result is not 1. Got: %d", result);
}

{
  int result = sum(2, 0);
  DSE_ASSERT(result == 2, "Result is not 2. Got: %d", result);
}

puts("Teardown();");
DSE_END_SUITE();

void sum_test() {
  int result = sum(1, 3);
  DSE_ASSERT(result == 4, "Result is not four. Got: %d", result);
}

void sum_test_1() {
  int result = sum(2, 3);
  result = 3;
  DSE_ASSERT(result == 5, "Result is not five. Got: %d", result);
}

void sum_test_2() {
  int result = sum(4, 3);
  result = 3;
  DSE_SKIP(DSE_ASSERT(result == 7));
}

void for_loop_test() {
  for(int i = 0; i < 3; i++) {
    DSE_ASSERT((i & 1) == 1, "i is not odd. Got %d", i);
  }
}

int main(int argc, char* argv[]) {
  puts("Running tests...\n");
  // sum_suite();
  sum_test();
  // sum_test_1();
  // sum_test_2();
  for_loop_test();
  // DSE_TIMER(for_loop_test);
  // DSE_TIMER(sum_suite);

  DSE_PRINT_RESULTS();

  //////////////////

  for(int i = 0; i < argc; i++) {
    printf("Arg %d -> %s\n", i, argv[i]);
  }
}