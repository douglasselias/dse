#include "code_a.c"

DSE_TEST(code_a_test_1,
  int result = sum(2, 2);
  DSE_ASSERT(result == 2, "Result is not 2. Got: %d", result);
);