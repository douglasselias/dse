#include "code_c_folder/nested/code_d.c"

DSE_TEST(code_a_test_1,
  int result = sum(1, 2);
  DSE_ASSERT(result == 3, "Result is not 3. Got: %d", result);
);